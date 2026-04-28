using System.Collections.Concurrent;
using System.Net.WebSockets;

using Test_Server.Handlers;

namespace Test_Server.Endpoints;

public static class WebSocketEndpoint
{
    public static ConcurrentDictionary<string, WebSocket> ConnectedSockets = new ConcurrentDictionary<string, WebSocket>();

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
                    context.Response.StatusCode = StatusCodes.Status401Unauthorized;
                    return;
                }

                var handler = context.RequestServices.GetRequiredService<SocketHandler>();
                var socket = await context.WebSockets.AcceptWebSocketAsync();
                ConnectedSockets[username] = socket;

                await handler.HandleAsync(socket, username);
            }
            else
            {
                context.Response.StatusCode = StatusCodes.Status400BadRequest;
            }
        });
        }
    }
}