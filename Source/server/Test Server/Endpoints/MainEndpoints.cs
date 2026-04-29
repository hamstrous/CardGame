using System.Collections.Concurrent;
using System.Threading.RateLimiting;
using Test_Server.Models;
using Test_Server.Services;
using Test_Server.Handlers;


namespace Test_Server.Endpoints;

public static class MainEndpoints
{
    public static void MapEndpoints(IEndpointRouteBuilder route)
    {
        route.MapPost("/login", async (HttpContext context) =>
        {
            var request = await context.Request.ReadFromJsonAsync<LoginRequest>();
            var playerService = context.RequestServices.GetRequiredService<PlayerService>();
            if (request is null)
            {
                return Results.BadRequest(new { error = "Invalid request body" });
            }
            var authToken = Guid.NewGuid().ToString();
            var player = new PlayerInfo { Username = request.Username, Password = request.Password, AuthToken = authToken };
            if (playerService.AddPlayerAndToken(authToken, player))
            {
                return Results.Ok(new { authToken });
            }

            return Results.BadRequest(new { error = "Failed to create player" });
        });

    }
}