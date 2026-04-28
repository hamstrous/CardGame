using System.Net.WebSockets;
using System.Security.Claims;
using System.Text;
using System.Text.Json;

using Test_Server.Endpoints;

namespace Test_Server.Handlers;


public class SocketHandler
{
    public async Task HandleAsync(WebSocket socket, string user)
    {
        var buffer = new byte[1024 * 4];
        Console.WriteLine($"[WebSocket] Connected: {user}");

        while (socket.State == WebSocketState.Open)
        {
            try
            {
                var result = await socket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

                if (result.MessageType == WebSocketMessageType.Close)
                {
                    Console.WriteLine($"[WebSocket] Disconnected: {user}");
                }
                else
                {
                    var message = Encoding.UTF8.GetString(buffer, 0, result.Count);
                    await HandleMessageAsync(socket, message);
                }
            }
            catch (WebSocketException)
            {
                Console.WriteLine($"[WebSocket] Connection error with {user}");
                await HandleCloseAsync(socket, user);
                break;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[WebSocket] Error with {user}: {ex.Message}");
                break;
            }
        }
    }

    private async Task HandleMessageAsync(WebSocket socket, string json)
    {
        try
        {
            using var doc = JsonDocument.Parse(json);
            var root = doc.RootElement;

            if (!root.TryGetProperty("cmd", out var cmdElement))
            {
                await SendErrorMessageAsync(socket, "Missing cmd property");
                return;
            }

            var cmd = cmdElement.GetString();
            try
            {
                switch (cmd)
                {
                    case "broadcast":
                        var data = root.GetProperty("data").GetString();
                        if (string.IsNullOrEmpty(data))
                        {
                            await SendErrorMessageAsync(socket, "Missing data property");
                            return;
                        }

                        foreach (var kvp in WebSocketEndpoint.connectedSockets)
                        {
                            if (kvp.Value != socket)
                            {
                                await SendMessageAsync(kvp.Value, data);
                            }
                        }
                        break;
                    default:
                        throw new InvalidOperationException($"Unknown command: {cmd}");
                }
            }
            catch (InvalidOperationException ex)
            {
                Console.WriteLine($"[WebSocket] '{cmd}' not found: {ex.Message}");
                await SendErrorMessageAsync(socket, "Unknown command");
            }
        }
        catch (JsonException ex)
        {
            Console.WriteLine($"[WebSocket] JSON parsing error from : {ex.Message}");
            await SendErrorMessageAsync(socket, "Invalid JSON format");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[WebSocket] Error handling message from : {ex.Message}");
            await SendErrorMessageAsync(socket, ex.Message);
        }
    }

    public async Task HandleCloseAsync(WebSocket socket, string session)
    {
        try
        {
            MainEndpoints.validUsers.TryRemove(session, out _);
            WebSocketEndpoint.connectedSockets.TryRemove(session, out _);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[WebSocket] Error during disconnect handling for {session}: {ex.Message}");
        }
        Console.WriteLine($"[WebSocket] Closing connection for {session}");
    }

    public static async Task SendMessageAsync(WebSocket socket, string message)
    {
        var buffer = Encoding.UTF8.GetBytes(message);
        await socket.SendAsync(buffer, WebSocketMessageType.Text, true, CancellationToken.None);
    }

    static public async Task SendErrorMessageAsync(WebSocket socket, string errorMessage)
    {
        var errorData = new
        {
            result = "ERROR",
            message = errorMessage
        };
        var errorResponse = JsonSerializer.Serialize(errorData);
        await SendMessageAsync(socket, errorResponse);
    }

    static public async Task SendSuccessMessageAsync(WebSocket socket, object data)
    {
        var successData = new
        {
            result = "SUCCESS",
            data
        };
        var successResponse = JsonSerializer.Serialize(successData);
        await SendMessageAsync(socket, successResponse);
    }

}