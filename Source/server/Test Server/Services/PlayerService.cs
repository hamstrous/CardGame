
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
        private readonly ConcurrentDictionary<string, RoomData> _rooms = new ConcurrentDictionary<string, RoomData>();

        public RoomData CreateRoom(string roomId, PlayerInfo host, int requiredPlayerCount)
        {
            var room = new RoomData
            {
                RoomId = roomId,
                Host = host,
                RequiredPlayerCount = requiredPlayerCount,
                Players = [host]
            };
            _rooms[roomId] = room;
            return room;
        }

        public RoomData? GetRoom(string roomId)
        {
            _rooms.TryGetValue(roomId, out var room);
            return room;
        }

        public void RemoveRoom(string roomId)
        {
            _rooms.TryRemove(roomId, out _);
        }

        public IEnumerable<RoomData> GetAllRooms()
        {
            return _rooms.Values;
        }

        public void RemovePlayerFromRoom(string roomId, PlayerInfo player)
        {
            if (_rooms.TryGetValue(roomId, out var room))
            {
                room.Players.Remove(player);
                if (room.Players.Count == 0)
                {
                    RemoveRoom(roomId);
                }
            }
        }


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