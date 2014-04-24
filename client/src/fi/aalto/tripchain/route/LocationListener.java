package fi.aalto.tripchain.route;

import android.content.Context;
import android.location.Location;
import android.util.Log;
import fi.aalto.tripchain.receivers.EventDispatcher;
import fi.aalto.tripchain.receivers.LocationReceiver;

public class LocationListener extends LocationReceiver {
	private Route route;
	
	private static final String TAG = LocationListener.class.getSimpleName();
	

	public LocationListener(Context context, Route route) {
		super(context);
		this.route = route;
	}
	
	@Override
	public void onLocationChanged(Location location) {
		Log.d(TAG, "Provider: " + location.getProvider()  + 
				" Accuracy: " + location.getAccuracy() +
				" Latitude: " + location.getLatitude() + 
				" Longitude: " + location.getLongitude());
		
		
		this.route.onLocation(location);
		EventDispatcher.onLocation(location);
	}
}
