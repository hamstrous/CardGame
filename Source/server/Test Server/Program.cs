using Test_Server.Endpoints;
using Test_Server.Handlers;
using Test_Server.Services;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddSingleton<PlayerService>();
builder.Services.AddScoped<SocketHandler>();
var app = builder.Build();

app.MapEndpoints();
app.MapAndUseWebSockets();
app.Run();
