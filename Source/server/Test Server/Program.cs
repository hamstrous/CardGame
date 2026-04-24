var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

var currentPlayerCount = 0;

app.MapGet("/", () =>
{
	currentPlayerCount++;
	Console.WriteLine("There are " + currentPlayerCount + " players. Current index: " + (currentPlayerCount - 1));
   
    return currentPlayerCount - 1;
});

app.Run();
