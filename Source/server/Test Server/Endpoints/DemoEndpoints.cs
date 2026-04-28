namespace Test_Server.Endpoints;

public static class DemoEndpoints
{
    public static void MapEndpoints(IEndpointRouteBuilder routeBuilder)
    {

        var _gameLock = new object();
        var currentPlayerCount = 0;
        var list = new List<int>();

        var previousPlayedCardIndex = -1;
        var previousPlayerId = -1;

        routeBuilder.MapGet("/", () =>
        {
            int index;
            lock (_gameLock)
            {
                currentPlayerCount++;
                index = currentPlayerCount - 1;
            }
            Console.WriteLine("There are " + (index + 1) + " players. Current index: " + index);

            return index;
        });

        routeBuilder.MapGet("/shuffle/{count:int}", (int count) =>
        {
            if (count <= 0) return new List<int>();

            List<int> currentList;
            lock (_gameLock)
            {
                if (list.Count != count)
                {
                    var random = new Random();
                    list = Enumerable.Range(0, count).OrderBy(_ => random.Next()).ToList();
                }
                currentList = list.ToList(); // Return a copy for thread safety
            }

            Console.WriteLine(string.Join(", ", currentList));
            return currentList;
        });

        routeBuilder.MapPost("/play/{playerId:int}/{cardId:int}", (int playerId, int cardId) =>
        {
            int playedCard;
            lock (_gameLock)
            {
                if (cardId < 0 || cardId >= list.Count) return "Invalid index";
                if (cardId == previousPlayedCardIndex) return "Card already played";
                if (playerId == previousPlayerId) return "Not your turn";

                previousPlayedCardIndex = cardId;
                previousPlayerId = playerId;
                playedCard = list[cardId];
            }

            Console.WriteLine($"Player {playerId} played card with index: {cardId}");
            return $"Player {playerId} played card: {playedCard}";
        });

        routeBuilder.MapGet("/play/{playerId:int}", async (int playerId) =>
        {
            int lastPlayedIndex;
            int lastPlayer;

            lock (_gameLock)
            {
                lastPlayedIndex = previousPlayedCardIndex;
                lastPlayer = previousPlayerId;
            }

            if (playerId == lastPlayer)
            {
                return Results.BadRequest("Not your turn");
            }

            if (lastPlayedIndex == -1) return Results.BadRequest("No card has been played yet");
            Console.WriteLine($"Player {playerId} is checking the last played card index: {lastPlayedIndex}");
            return Results.Ok(lastPlayedIndex);
        });

    }
}