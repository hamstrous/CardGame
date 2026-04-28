using System.Collections.Concurrent;
using System.Threading.RateLimiting;
using Test_Server.Models;
using Test_Server.Handlers;


namespace Test_Server.Endpoints;

public static class MainEndpoints
{
    public static ConcurrentDictionary<string, string> validUsers = new ConcurrentDictionary<string, string>();
    public static void MapEndpoints(IEndpointRouteBuilder route)
    {
        route.MapPost("/login", async (HttpContext context) =>
        {
            var request = await context.Request.ReadFromJsonAsync<LoginRequest>();
            if (request is null)
            {
                context.Response.StatusCode = 400;
                await context.Response.WriteAsJsonAsync(new { error = "Invalid request body" });
                return Results.BadRequest();
            }
            var authToken = Guid.NewGuid().ToString();
            validUsers[authToken] = request.Username;
            await context.Response.WriteAsJsonAsync(new { authToken });
            return Results.Ok();
        });


    }
}