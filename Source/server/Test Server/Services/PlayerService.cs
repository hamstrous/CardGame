
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.WebSockets;
using System.Security.Claims;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using Test_Server.Handlers;
using Test_Server.Models;

namespace Test_Server.Services
{
    public class PlayerService
    {
        private readonly ConcurrentDictionary<string, PlayerInfo> _players = new ConcurrentDictionary<string, PlayerInfo>();
        private readonly HashSet<string> _validAuthTokens = new HashSet<string>();

        public bool AddPlayerAndToken(string authToken, PlayerInfo player)
        {
            if (_validAuthTokens.Contains(authToken))
            {
                return false;
            }

            _players[authToken] = player;
            _validAuthTokens.Add(authToken);
            return true;
        }

        public bool IsValidToken(string authToken)
        {
            return _validAuthTokens.Contains(authToken);
        }

        public PlayerInfo? GetPlayer(string authToken)
        {
            _players.TryGetValue(authToken, out var player);
            if (player == null)
            {
                Debug.WriteLine($"[PlayerService] No player found for token: {authToken}");
                _validAuthTokens.Remove(authToken);
                return null;
            }
            return player;
        }

        public void RemovePlayerAndToken(string authToken)
        {
            _validAuthTokens.Remove(authToken);
            _players.TryRemove(authToken, out _);
        }

        public IEnumerable<KeyValuePair<string, PlayerInfo>> GetAllPlayers()
        {
            return _players;
        }
    }
}