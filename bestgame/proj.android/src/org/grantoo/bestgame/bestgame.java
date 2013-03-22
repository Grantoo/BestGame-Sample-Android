package org.grantoo.bestgame;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.grantoo.lib.propeller.PropellerSDK;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;

/*******************************************************************************
 * Cocos2d-x Android Activity entry point.
 */
public class bestgame extends Cocos2dxActivity {

	private static Cocos2dxActivity bestgameActivity = null;

	/***************************************************************************
	 * Static initializer to load the "C" library.
	 */
	static {
		System.loadLibrary("bestgame");
	}

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
		Log.v("Bestgame", "onCreate");
		super.onCreate(savedInstanceState);

		setVolumeControlStream(AudioManager.STREAM_MUSIC);

		PropellerSDK.onCreate(this);
	}

	/***************************************************************************
	 * Called when an activity you launched exits, giving you the requestCode
	 * you started it with, the resultCode it returned, and any additional data
	 * from it. The resultCode will be RESULT_CANCELED if the activity
	 * explicitly returned that, didn't return any result, or crashed during its
	 * operation.
	 * 
	 * You will receive this call immediately before onResume() when your
	 * activity is re-starting.
	 * 
	 * @param requestCode The integer request code originally supplied to
	 *        startActivityForResult(), allowing you to identify who this result
	 *        came from.
	 * @param resultCode The integer result code returned by the child activity
	 *        through its setResult().
	 * @param data An Intent, which can return result data to the caller
	 *        (various data can be attached to Intent "extras").
	 */
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.v("Bestgame", "onActivityResult");
		super.onActivityResult(requestCode, resultCode, data);
		PropellerSDK.onActivityResult(this, requestCode, resultCode, data);
	}

	/***************************************************************************
	 * Called after onRestoreInstanceState(Bundle), onRestart(), or onPause(),
	 * for your activity to start interacting with the user. This is a good
	 * place to begin animations, open exclusive-access devices (such as the
	 * camera), etc.
	 * 
	 * Keep in mind that onResume is not the best indicator that your activity
	 * is visible to the user; a system window such as the keyguard may be in
	 * front. Use onWindowFocusChanged(boolean) to know for certain that your
	 * activity is visible to the user (for example, to resume a game).
	 * 
	 * Derived classes must call through to the super class's implementation of
	 * this method. If they do not, an exception will be thrown.
	 */
	@Override
	protected void onResume() {
		Log.v("Bestgame", "onResume");
		super.onResume();
		bestgameActivity = this;
		PropellerSDK.onResume(this);
	}

	public static void sendFeedback() {
		Log.v("Bestgame", "sendFeedback");
		Intent i = new Intent(Intent.ACTION_SEND);
		i.setType("message/rfc822");
		i.putExtra(Intent.EXTRA_EMAIL, new String[] {
			"support@grantoo.org"
		});
		i.putExtra(Intent.EXTRA_SUBJECT, "Feedback for bestgame");
		i.putExtra(Intent.EXTRA_TEXT, "Enter your feedback here");

		bestgameActivity.startActivity(Intent.createChooser(i, "Send feedback using..."));
	}

}