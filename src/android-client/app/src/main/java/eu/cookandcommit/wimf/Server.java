package eu.cookandcommit.wimf;

import com.google.android.gms.maps.model.LatLng;

import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

interface ProtocolListener {
    void userLocationChanged(User user, LatLng location);
    void userRegistered(int id, String nick);
}

public class Server {
    List<ProtocolListener> listeners = new ArrayList<ProtocolListener>();
    Socket client = null;

    public void addListener(ProtocolListener listener) {
        listeners.add(listener);
    }

    public void requestClients(LatLng location) {
    }

    public boolean connect(String host, int port) {
        assert host != null;
        try {
            client = new Socket(host, port);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public boolean isConnected() {
        return client != null && client.isConnected();
    }

    public void registerRequest(String nick) {
    }

    public void disconnect() {
        if (isConnected()) {
            try {
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
