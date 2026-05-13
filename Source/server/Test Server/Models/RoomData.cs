namespace Test_Server.Models;

public class RoomData
{
    public required string RoomId { get; set; }
    public required PlayerInfo Host { get; set; }
    public int RequiredPlayerCount { get; set; }
    public List<PlayerInfo> Players { get; set; } = new List<PlayerInfo>();
}