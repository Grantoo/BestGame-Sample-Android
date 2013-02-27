package org.grantoo.bestgame;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.content.Intent;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;

/*******************************************************************************
 * Cocos2d-x Android Activity entry point.
 */
public class bestgame extends Cocos2dxActivity {

	private static Cocos2dxActivity sBestgameActivity = null;

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
		sBestgameActivity = this;
	}

	/***************************************************************************
	 * Posts an intent to send game feedback.
	 */
	public static void sendFeedback() {
		Log.v("Bestgame", "sendFeedback");

		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("message/rfc822");
		intent.putExtra(
			Intent.EXTRA_EMAIL,
			new String[] {
				"support@grantoo.org"
			});
		intent.putExtra(Intent.EXTRA_SUBJECT, "Feedback for bestgame");
		intent.putExtra(Intent.EXTRA_TEXT, "Enter your feedback here");

		Intent chooserIntent = Intent.createChooser(
			intent,
			"Send feedback using...");

		sBestgameActivity.startActivity(chooserIntent);
	}

}