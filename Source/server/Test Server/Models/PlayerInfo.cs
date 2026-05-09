using System.Net.WebSockets;

namespace Test_Server.Models;

public class PlayerInfo
{
    public required string Username { get; set; }
    public required string Password { get; set; }

    public string AuthToken { get; set; } = string.Empty;
    public WebSocket? Socket { get; set; }

    public string CurrentRoomId { get; set; } = string.Empty;
    public bool IsHost { get; set; } = false;
    public int Index { get; set; } = 0; // 0 is host
    public int PlayerCount { get; set; } = 0; // Total players in the room, including self
    public void Clear()
    {
        AuthToken = string.Empty;
        Socket = null;
        CurrentRoomId = string.Empty;
        IsHost = false;
    }
}