using System.Net.WebSockets;
using System.Security.Claims;
using System.Text;
using System.Text.Json;

using Test_Server.Endpoints;
using Test_Server.Models;
using Test_Server.Services;

namespace Test_Server.Handlers;


public class SocketHandler(PlayerService playerService)
{
    public async Task HandleAsync(WebSocket socket, PlayerInfo user)
    {
        var buffer = new byte[1024 * 4];
        Console.WriteLine($"[WebSocket] Connected: {user.Username}");

        while (socket.State == WebSocketState.Open)
        {
            try
            {
                var result = await socket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

                if (result.MessageType == WebSocketMessageType.Close)
                {
                    Console.WriteLine($"[WebSocket] Disconnected: {user.Username}");
                }
                else
                {
                    var message = Encoding.UTF8.GetString(buffer, 0, result.Count);
                    await HandleMessageAsync(socket, message, user);
                }
            }
            catch (WebSocketException)
            {
                Console.WriteLine($"[WebSocket] Connection error with {user.Username}");
                await HandleCloseAsync(socket, user);
                break;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[WebSocket] Error with {user.Username}: {ex.Message}");
                break;
            }
        }
    }

    private async Task HandleMessageAsync(WebSocket socket, string json, PlayerInfo user)
    {
        try
        {
            var message = JsonSerializer.Deserialize<WebSocketMessage>(json);
            Console.WriteLine($"[WebSocket] Received message from {user.Username}: {json}");

            var data = message?.Data;

            var cmd = message?.Command;
            try
            {
                switch (cmd)
                {
                    case "broadcast":
                        if (data == null)
                        {
                            await SendErrorMessageAsync(socket, "Missing data property");
                            return;
                        }

                        if (string.IsNullOrEmpty(user.CurrentRoomId))
                        {
                            await SendErrorMessageAsync(socket, "User must be in a room to broadcast messages");
                            return;
                        }

                        foreach (var kvp in playerService.GetAllPlayers())
                        {
                            if (kvp.Value.Socket != null && kvp.Value.Socket.State == WebSocketState.Open && kvp.Value.Socket != socket && kvp.Value.CurrentRoomId == user.CurrentRoomId)
                            {
                                var broadcastData = new
                                {
                                    type = "broadcast",
                                    command = "broadcast",
                                    data,
                                    time_stamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds(),
                                    from = user.Username
                                };
                                await SendMessageAsync(kvp.Value.Socket, broadcastData);
                            }
                        }
                        break;
                    case "create_room":
                        var createdRoomId = Guid.NewGuid().ToString("N");
                        user.CurrentRoomId = createdRoomId;
                        var roomData = new
                        {
                            type = "response",
                            command = "create_room",
                            data = new
                            {
                                room_id = createdRoomId
                            },
                            time_stamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds()
                        };
                        await SendMessageAsync(socket, roomData);
                        break;
                    case "join_room":
                        if (data == null)
                        {
                            await SendErrorMessageAsync(socket, "Missing data property");
                            return;
                        }
                        var joinedRoomId = data.Value.GetProperty("room_id").GetString();
                        if (joinedRoomId == null)
                        {
                            await SendErrorMessageAsync(socket, "Missing room_id property");
                            return;
                        }
                        user.CurrentRoomId = joinedRoomId;

                        var joinRoomData = new
                        {
                            type = "response",
                            command = "join_room",
                            data = new
                            {
                                room_id = joinedRoomId
                            },
                            time_stamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds()
                        };
                        await SendMessageAsync(socket, joinRoomData);
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
            Console.WriteLine($"[WebSocket] JSON parsing error from {user.Username}: {ex.Message}");
            await SendErrorMessageAsync(socket, "Invalid JSON format");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[WebSocket] Error handling message from {user.Username}: {ex.Message}");
            await SendErrorMessageAsync(socket, ex.Message);
        }
    }

    public async Task HandleCloseAsync(WebSocket socket, PlayerInfo user)
    {
        try
        {
            playerService.RemovePlayerAndToken(user.AuthToken);
            user.Socket = null;
            user.Clear();
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[WebSocket] Error during disconnect handling for {user.Username}: {ex.Message}");
        }
        Console.WriteLine($"[WebSocket] Closing connection for {user.Username}");
    }

    public static async Task SendMessageAsync(WebSocket socket, string message)
    {
        var buffer = Encoding.UTF8.GetBytes(message);
        await socket.SendAsync(buffer, WebSocketMessageType.Text, true, CancellationToken.None);
    }

    public static async Task SendMessageAsync(WebSocket socket, object data)
    {
        var json = JsonSerializer.Serialize(data);
        await SendMessageAsync(socket, json);
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