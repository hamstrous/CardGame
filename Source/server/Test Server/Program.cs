using Test_Server.Endpoints;
using Test_Server.Handlers;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddSingleton<SocketHandler>();
var app = builder.Build();

app.MapEndpoints();
app.MapAndUseWebSockets();
app.Run();
