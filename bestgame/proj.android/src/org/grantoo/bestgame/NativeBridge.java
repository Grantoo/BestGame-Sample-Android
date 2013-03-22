package org.grantoo.bestgame;

import org.grantoo.lib.propeller.PropellerSDK;
import org.grantoo.lib.propeller.PropellerSDKListener;
import org.grantoo.lib.propeller.PropellerSDKUtil;
import org.json.JSONException;
import org.json.JSONObject;
import android.os.Bundle;
import android.util.Log;

/*******************************************************************************
 * Bridging class between the Propeller SDK and the native code on the Java
 * side.
 */
public class NativeBridge implements PropellerSDKListener {

	/***************************************************************************
	 * Initializes the Propeller SDK.
	 * 
	 * @param gameId Game ID to initialize the Propeller SDK with.
	 * @param gameSecret Game secret token to initialize the Propeller SDK with.
	 */
	public static void initialize(String gameId, String gameSecret) {
		PropellerSDK.initialize(gameId, gameSecret);
	}

	/***************************************************************************
	 * Preinitialization of the Propeller SDK with Test values
	 * 
	 */
	public static void useSandboxServers() {
		PropellerSDK.useSandboxServers();
	}

	/***************************************************************************
	 * Preinitialization of the Propeller SDK with Debug values
	 * 
	 * @param onlineSdkUrl Online SDK server URL to initialize the Propeller SDK
	 *        with.
	 * @param grantooApiUrl Grantoo API server URL to initialize the Propeller
	 *        SDK with.
	 * @param tournamentApiUrl Tournament API server URL to initialize the
	 *        Propeller SDK with.
	 * @param challengeApiUrl Challenge API server URL to initialize the
	 *        Propeller SDK with.
	 */
	public static void useDebugServers(String onlineSdkUrl, String grantooApiUrl, String tournamentApiUrl, String challengeApiUrl) {
		PropellerSDK.useDebugServers(onlineSdkUrl, grantooApiUrl, tournamentApiUrl, challengeApiUrl);
	}

	/***************************************************************************
	 * Set the notification token (GCM registration ID) to use in identifying
	 * the user's device for sending push notifications to.
	 * 
	 * @param notificationToken The string representing the notification token.
	 */
	public static void setNotificationToken(String notificationToken) {
		PropellerSDK.setNotificationToken(notificationToken);
	}

	/***************************************************************************
	 * Invoked when the garbage collector has detected that this instance is no
	 * longer reachable. The default implementation does nothing, but this
	 * method can be overridden to free resources.
	 * 
	 * Note that objects that override finalize are significantly more expensive
	 * than objects that don't. Finalizers may be run a long time after the
	 * object is no longer reachable, depending on memory pressure, so it's a
	 * bad idea to rely on them for cleanup. Note also that finalizers are run
	 * on a single VM-wide finalizer thread, so doing blocking work in a
	 * finalizer is a bad idea. A finalizer is usually only necessary for a
	 * class that has a native peer and needs to call a native method to destroy
	 * that peer. Even then, it's better to provide an explicit close method
	 * (and implement Closeable), and insist that callers manually dispose of
	 * instances. This works well for something like files, but less well for
	 * something like a BigInteger where typical calling code would have to deal
	 * with lots of temporaries. Unfortunately, code that creates lots of
	 * temporaries is the worst kind of code from the point of view of the
	 * single finalizer thread.
	 * 
	 * If you must use finalizers, consider at least providing your own
	 * ReferenceQueue and having your own thread process that queue.
	 * 
	 * Unlike constructors, finalizers are not automatically chained. You are
	 * responsible for calling super.finalize() yourself.
	 * 
	 * Uncaught exceptions thrown by finalizers are ignored and do not terminate
	 * the finalizer thread. See Effective Java Item 7, "Avoid finalizers" for
	 * more.
	 * 
	 * @throws Throwable thrown whenever an exception occurs during
	 *         finalization.
	 */
	@Override
	protected void finalize() throws Throwable {
		deallocateJNIBridge();
		super.finalize();
	}

	/***************************************************************************
	 * Set the Propeller SDK Activity orientation.
	 * 
	 * @param orientation The Propeller SDK Activity orientation. Must either be
	 *        "landscape" or "portrait".
	 */
	public void setOrientation(String orientation) {
		PropellerSDK.instance().setOrientation(orientation);
	}

	/***************************************************************************
	 * Launches the Propeller SDK web view.
	 * 
	 * @return True if the Propeller SDK web view was successfully launched,
	 *         false otherwise.
	 */
	public boolean launch() {
		return PropellerSDK.instance().launch(this);
	}

	/***************************************************************************
	 * Launches the Propeller SDK web view for the given tournament id.
	 * 
	 * @param tournamentId Tournament id.
	 * @return True if the Propeller SDK web view was successfully launched,
	 *         false otherwise.
	 */
	public boolean launchWithTournament(String tournamentId) {
		return PropellerSDK.instance().launchWithTournament(tournamentId, this);
	}

	/***************************************************************************
	 * Launches the Propeller SDK web view with the given match result.
	 * 
	 * @param tournamentId Tournament ID of the match result.
	 * @param matchId Match ID of the match result.
	 * @param score Score of the match result.
	 * @return True if the Propeller SDK web view was successfully launched,
	 *         false otherwise.
	 */
	public boolean launchWithMatchResult(String tournamentId, String matchId, long score) {
		return PropellerSDK.instance().launchWithMatchResult(tournamentId, matchId, score, this);
	}

	/***************************************************************************
	 * Sync the current users challenge counts
	 */
	public void syncChallengeCounts() {
		PropellerSDK.instance().syncChallengeCounts();
	}

	/***************************************************************************
	 * Get the current users last known challenge counts
	 */
	public int getChallengeCounts() {
		return PropellerSDK.instance().getChallengeCounts();
	}

	/***************************************************************************
	 * Called when the Propeller SDK completed with exit.
	 */
	@Override
	public void sdkCompletedWithExit() {
		nativeSdkCompletedWithExit();
	}

	/***************************************************************************
	 * Called when the Propeller SDK completed with a match.
	 * 
	 * @param tournamentId Tournament ID of the match.
	 * @param matchID Match ID of the match.
	 * @param params JSON encoded string of game context match parameters.
	 */
	@Override
	public void sdkCompletedWithMatch(String tournamentId, String matchId, String params) {
		try {
			JSONObject paramsJSON = new JSONObject(params);
			String seedString = paramsJSON.getString("seed");
			String optionsString = paramsJSON.getString("options");
			String roundString = paramsJSON.getString("round");

			long seed = Long.parseLong(seedString);
			int round = Integer.parseInt(roundString);

			JSONObject optionsJSON = new JSONObject(optionsString);
			String gameTypeString = optionsJSON.getString("gametype");

			int gameType = Integer.parseInt(gameTypeString);

			nativeSdkCompletedWithMatch(tournamentId, matchId, seed, round, gameType);
		} catch (JSONException jsonException) {
			Log.e(PropellerSDKUtil.PSDK_LOG_TAG, jsonException.getMessage());
		}
	}

	/***************************************************************************
	 * Called when the Propeller SDK failed.
	 * 
	 * @param message Failure message.
	 * @param result Failed result returned by the Propeller SDK.
	 */
	@Override
	public void sdkFailed(String message, Bundle result) {
		nativeSdkFailed(message, result);
	}

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK completed with exit.
	 */
	public native void nativeSdkCompletedWithExit();

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK completed with a
	 * match.
	 * 
	 * @param tournamentId Tournament ID of the match.
	 * @param matchID Match ID of the match.
	 * @param seed Random seed to use for the match.
	 * @param round Current match round.
	 * @param gameType Game type of the match.
	 */
	public native void nativeSdkCompletedWithMatch(String tournamentId, String matchId, long seed, int round, int gameType);

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK failed.
	 * 
	 * @param message Failure message.
	 * @param result Failed result returned by the Propeller SDK.
	 */
	public native void nativeSdkFailed(String message, Bundle result);

	/***************************************************************************
	 * Notifies the native libraries to deallocate global references in the JNI
	 * bridge.
	 */
	public native void deallocateJNIBridge();

}