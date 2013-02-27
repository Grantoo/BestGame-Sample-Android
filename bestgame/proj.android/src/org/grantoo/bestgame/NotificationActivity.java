package org.grantoo.bestgame;

import org.grantoo.lib.propeller.PropellerSDK;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

/*******************************************************************************
 * Dummy activity used to help bring the app into the foreground if it is
 * running in the background.
 */
public class NotificationActivity extends Activity {

	/***************************************************************************
	 * Called when the activity is starting. This is where most initialization
	 * should go: calling setContentView(int) to inflate the activity's UI,
	 * using findViewById(int) to programmatically interact with widgets in the
	 * UI, calling managedQuery(android.net.Uri, String[], String, String[],
	 * String) to retrieve cursors for data being displayed, etc.
	 * 
	 * You can call finish() from within this function, in which case
	 * onDestroy() will be immediately called without any of the rest of the
	 * activity lifecycle (onStart(), onResume(), onPause(), etc) executing.
	 * 
	 * @param savedInstanceState If the activity is being re-initialized after
	 *        previously being shut down then this Bundle contains the data it
	 *        most recently supplied in onSaveInstanceState(Bundle). Note:
	 *        Otherwise it is null.
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Bundle data = getIntent().getExtras();
		boolean newLaunch = isTaskRoot();

		PropellerSDK.handleRemoteNotification(data, newLaunch);

		if (newLaunch) {
			Intent intent = new Intent(this, bestgame.class);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

			startActivity(intent);
		}

		finish();
	}

}