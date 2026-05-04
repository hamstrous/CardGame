using System.Text.Json;
using System.Text.Json.Serialization;

public class WebSocketMessage
{
    [JsonPropertyName("type")]
    public required string Type { get; set; }
    [JsonPropertyName("command")]
    public required string Command { get; set; }
    [JsonPropertyName("id")]
    public int Id { get; set; }

    [JsonPropertyName("time_stamp")]
    public int TimeStamp { get; set; }

    [JsonPropertyName("data")]
    public JsonElement? Data { get; set; }
}