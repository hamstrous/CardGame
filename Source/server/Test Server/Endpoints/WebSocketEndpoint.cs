using System.Collections.Concurrent;
using System.Net.WebSockets;

using Test_Server.Handlers;
using Test_Server.Services;

namespace Test_Server.Endpoints;

public static class WebSocketEndpoint
{
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
                    var playerService = context.RequestServices.GetRequiredService<PlayerService>();
                    var playerInfo = playerService.GetPlayer(authToken);
                    if (playerInfo == null)
                    {
                        return Results.Unauthorized();
                    }

                    var handler = context.RequestServices.GetRequiredService<SocketHandler>();
                    var socket = await context.WebSockets.AcceptWebSocketAsync();
                    playerInfo.Socket = socket;

                    await handler.HandleAsync(socket, playerInfo);
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