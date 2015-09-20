package eu.onionsoftware.wimf;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.model.LatLng;

public class MainActivity extends FragmentActivity implements OnMapReadyCallback, ProtocolListener {

    private GoogleMap map = null;
    private Server server;

    private boolean mapIsReady() {
        return map != null;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MapFragment mapFragment = (MapFragment) getFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        WimfApplication app = (WimfApplication) getApplication();
        assert app != null;

        server = app.getServer();
        // todo check if connection is established
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        map = googleMap;
    }

    @Override
    public void userLocationChanged(User user, LatLng location) {
        if (!mapIsReady()) {
            return;
        }

        /* todo
        map.addMarker(new MarkerOptions()
                .position(location)
                .title("Todo"));
                */
    }

    @Override
    public void userRegistered(int id, String nick) {
        Wimf.WimfInfo info = Wimf.WimfInfo.newBuilder().build();
    }
}
