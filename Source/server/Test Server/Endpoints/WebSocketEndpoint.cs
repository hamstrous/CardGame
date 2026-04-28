using System.Collections.Concurrent;
using System.Net.WebSockets;

using Test_Server.Handlers;

namespace Test_Server.Endpoints;

public static class WebSocketEndpoint
{
    public static ConcurrentDictionary<string, WebSocket> connectedSockets = new ConcurrentDictionary<string, WebSocket>();

    extension(WebApplication app)
    {
        public void MapAndUseWebSockets()
        {
            var options = new WebSocketOptions
            {
                KeepAliveInterval = TimeSpan.FromSeconds(120),
            };
            app.UseWebSockets(options);

            app.MapGet("/ws", async (HttpContext context) =>
            {
                if (context.WebSockets.IsWebSocketRequest)
                {
                    var authToken = context.Request.Headers["Authorization"].ToString();
                    var username = MainEndpoints.validUsers.GetValueOrDefault(authToken);
                    if (string.IsNullOrEmpty(username))
                    {
                        return Results.Unauthorized();
                    }

                    var handler = context.RequestServices.GetRequiredService<SocketHandler>();
                    var socket = await context.WebSockets.AcceptWebSocketAsync();
                    connectedSockets[username] = socket;

                    await handler.HandleAsync(socket, username);
                    return Results.Empty;
                }
                else
                {
                    return Results.BadRequest();
                }
            });
        }
    }
}