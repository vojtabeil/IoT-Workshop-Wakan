namespace MqttView.Data;

using System.Text;

using MQTTnet;
using MQTTnet.Client;

public class MqttService : IAsyncDisposable
{
    public const int NUMBER_OF_STATIONS = 5;

    public IDictionary<string, string> Clients = new Dictionary<string, string>();

    private IMqttClient? client;

    public event EventHandler? StateUpdated;

    public MqttService()
    {
        for (int i = 1; i <= NUMBER_OF_STATIONS; i++)
        {
            this.Clients[$"station-{i}"] = string.Empty;
        }
    }

    public async Task Initialize()
    {
        var mqttFactory = new MqttFactory();
        var mqttClient = mqttFactory.CreateMqttClient();
        var mqttClientOptions = new MqttClientOptionsBuilder()
            .WithTcpServer("192.168.2.1")
            .Build();

        mqttClient.ApplicationMessageReceivedAsync += async e =>
            {
                string data = string.Empty;
                var bytes = e.ApplicationMessage.PayloadSegment.ToArray();

                try
                {
                    data = Encoding.UTF8.GetString(bytes);
                }
                catch
                {
                    data = "<error>";
                }

                this.Clients[e.ApplicationMessage.Topic] = data;
                this.OnStateUpdated();
                await Task.CompletedTask;
            };

        await mqttClient.ConnectAsync(mqttClientOptions, CancellationToken.None);

        for (int i = 1; i <= NUMBER_OF_STATIONS; i++)
        {
            await mqttClient.SubscribeAsync($"station-{i}");
        }

        this.client = mqttClient;
    }

    public async ValueTask DisposeAsync()
    {
        this.client?.Dispose();
        await Task.CompletedTask;
    }

    protected virtual void OnStateUpdated()
    {
        this.StateUpdated?.Invoke(this, EventArgs.Empty);
    }
}
