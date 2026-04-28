namespace Test_Server.Endpoints;

public static class RootEndpoints
{
    extension(IEndpointRouteBuilder routeBuilder)
    {
        public void MapEndpoints()
        {
            DemoEndpoints.MapEndpoints(routeBuilder);
            MainEndpoints.MapEndpoints(routeBuilder);
        }
    }
}