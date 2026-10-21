using System.Text.Json.Serialization;

namespace Test_Server.Models;

public class LoginRequest
{
    [JsonPropertyName("username")]
    public required string Username { get; set; }
    [JsonPropertyName("password")]
    public required string Password { get; set; }
}