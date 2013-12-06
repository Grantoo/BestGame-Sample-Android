package org.grantoo.bestgame;

import java.util.Map;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.grantoo.lib.propeller.PropellerSDK;
import org.grantoo.lib.propeller.PropellerSDKBroadcastReceiver;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

/*******************************************************************************
 * Cocos2d-x Android Activity entry point.
 */
public class bestgame extends Cocos2dxActivity {

	private static Cocos2dxActivity sBestgameActivity = null;

	private IntentFilter mIntentFilter;

	private BroadcastReceiver mBroadcastReceiver;

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

		PropellerSDK.onCreate(this);

		setVolumeControlStream(AudioManager.STREAM_MUSIC);

		mIntentFilter = new IntentFilter();
		mIntentFilter.addAction("PropellerSDKChallengeCountChanged");
		mIntentFilter.addAction("PropellerSDKTournamentInfo");

		mBroadcastReceiver = getBroadcastReceiver();
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
		sBestgameActivity = this;

		PropellerSDK.onResume(this);

		LocalBroadcastManager.getInstance(this).registerReceiver(
			mBroadcastReceiver,
			mIntentFilter);
	}

	/***************************************************************************
	 * Called as part of the activity lifecycle when an activity is going into
	 * the background, but has not (yet) been killed. The counterpart to
	 * onResume().
	 * 
	 * When activity B is launched in front of activity A, this callback will be
	 * invoked on A. B will not be created until A's onPause() returns, so be
	 * sure to not do anything lengthy here.
	 * 
	 * This callback is mostly used for saving any persistent state the activity
	 * is editing, to present a "edit in place" model to the user and making
	 * sure nothing is lost if there are not enough resources to start the new
	 * activity without first killing this one. This is also a good place to do
	 * things like stop animations and other things that consume a noticeable
	 * amount of CPU in order to make the switch to the next activity as fast as
	 * possible, or to close resources that are exclusive access such as the
	 * camera.
	 * 
	 * In situations where the system needs more memory it may kill paused
	 * processes to reclaim resources. Because of this, you should be sure that
	 * all of your state is saved by the time you return from this function. In
	 * general onSaveInstanceState(Bundle) is used to save per-instance state in
	 * the activity and this method is used to store global persistent data (in
	 * content providers, files, etc.)
	 * 
	 * After receiving this call you will usually receive a following call to
	 * onStop() (after the next activity has been resumed and displayed),
	 * however in some cases there will be a direct call back to onResume()
	 * without going through the stopped state.
	 * 
	 * Derived classes must call through to the super class's implementation of
	 * this method. If they do not, an exception will be thrown.
	 */
	@Override
	protected void onPause() {
		Log.v("Bestgame", "onPause");
		super.onPause();

		LocalBroadcastManager.getInstance(this).unregisterReceiver(mBroadcastReceiver);
	}

	/***************************************************************************
	 * Retrieves the broadcast receiver used to listen to Propeller SDK local
	 * broadcast intents.
	 * 
	 * @return Broadcast receiver used to listen to Propeller SDK local
	 *         broadcast intents.
	 */
	private BroadcastReceiver getBroadcastReceiver() {
		return new PropellerSDKBroadcastReceiver() {

			@Override
			public void onReceive(Context context, String action, Map<String, Object> data) {
				if (action.equals("PropellerSDKChallengeCountChanged")) {
					int count = 0;
					Object countObject = data.get("count");

					if ((countObject != null) &&
						(countObject instanceof Integer)) {
						count = ((Integer) countObject).intValue();
					}

					NativeBridge.nativeUpdatedChallengeCount(count);
				} else if (action.equals("PropellerSDKTournamentInfo")) {
					String name = "";
					Object nameObject = data.get("name");

					if ((nameObject != null) &&
						(nameObject instanceof String)) {
						name = (String) nameObject;
					}

					String campaignName = "";
					Object campaignNameObject = data.get("campaignName");

					if ((campaignNameObject != null) &&
						(campaignNameObject instanceof String)) {
						campaignName = (String) campaignNameObject;
					}

					String sponsorName = "";
					Object sponsorNameObject = data.get("sponsorName");

					if ((sponsorNameObject != null) &&
						(sponsorNameObject instanceof String)) {
						sponsorName = (String) sponsorNameObject;
					}

					long startDate = 0;
					Object startDateObject = data.get("startDate");

					if ((startDateObject != null) &&
						(startDateObject instanceof Long)) {
						startDate = ((Long) startDateObject).longValue();
					}

					long endDate = 0;
					Object endDateObject = data.get("endDate");

					if ((endDateObject != null) &&
						(endDateObject instanceof Long)) {
						endDate = ((Long) endDateObject).longValue();
					}

					String logo = "";
					Object logoObject = data.get("logo");

					if ((logoObject != null) &&
						(logoObject instanceof String)) {
						logo = (String) logoObject;
					}

					NativeBridge.nativeUpdatedTournamentInfo(
						name,
						campaignName,
						sponsorName,
						startDate,
						endDate,
						logo);
				}
			}
		};
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

		sBestgameActivity.startActivity(Intent.createChooser(intent, "Send feedback using..."));
	}

}
