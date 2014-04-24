package fi.aalto.tripchain;

import java.util.UUID;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import android.app.Activity;
import android.os.Bundle;
import android.os.RemoteException;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

public class StartFragment extends Fragment {
	
	private Button startButton;
	private MainActivity main;
	
	private final static UUID PEBBLE_APP_UUID = UUID.fromString("3b760d02-93f3-4c0d-aea3-687a466eaab3");

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.start_fragment, container, false);
        
        this.main = (MainActivity) getActivity();
        
        this.startButton = ((Button) rootView.findViewById(R.id.button));
		this.startButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View arg0) {
				if (!main.recording) {
					try {
						main.serviceConnectionApi.start();
						main.recording = true;
						
						main.sendMessageToPebble(2, "Tracking started!");
						
					} catch (RemoteException e) {
					}
				} else {
					try {
						main.serviceConnectionApi.stop();
						main.recording = false;
						main.sendMessageToPebble(2, "Tracking stopped!");
						
					} catch (RemoteException e) {
					}
				}
				
				startButton.setText(!main.recording ? "Start recording"
						: "Stop recording");
			}
		});

		this.startButton.setText(!main.recording ? "Start recording"
				: "Stop recording");
  
        return rootView;
	}
}
