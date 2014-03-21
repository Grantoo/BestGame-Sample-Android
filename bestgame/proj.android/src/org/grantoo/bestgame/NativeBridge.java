package org.grantoo.bestgame;

import java.util.HashMap;
import java.util.Map;
import org.grantoo.lib.propeller.PropellerSDK;
import org.grantoo.lib.propeller.PropellerSDKListener;
import android.content.Context;

/*******************************************************************************
 * Bridging class between the Propeller SDK and the native code on the Java
 * side.
 */
public class NativeBridge extends PropellerSDKListener {

	/***************************************************************************
	 * Preinitialization of the Propeller SDK with Test values
	 * 
	 */
	public static void useSandbox() {
		PropellerSDK.useSandbox();
	}

	/***************************************************************************
	 * Initializes the Propeller SDK.
	 * 
	 * @param gameId Game ID to initialize the Propeller SDK with.
	 * @param gameSecret Game secret token to initialize the Propeller SDK with.
	 * @param gameHasLogin Flags whether or not the Propeller SDK should use the
	 *        integrated social provider for federated login.
	 * @param gameHasInvite Flags whether or not the Propeller SDK should use
	 *        the integrated social provider for invites.
	 * @param gameHasShare Flags whether or not the Propeller SDK should use hte
	 *        integrated social provider for sharing.
	 */
	public static void initialize(String gameId, String gameSecret, boolean gameHasLogin, boolean gameHasInvite, boolean gameHasShare) {
		PropellerSDK.initialize(
			gameId,
			gameSecret,
			gameHasLogin,
			gameHasInvite,
			gameHasShare);
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
	 * Launches the Propeller SDK web view with the given match result.
	 * 
	 * @param tournamentId Tournament ID of the match result.
	 * @param matchId Match ID of the match result.
	 * @param score Score of the match result.
	 * @return True if the Propeller SDK web view was successfully launched,
	 *         false otherwise.
	 */
	public boolean launchWithMatchResult(String tournamentId, String matchId, long score) {
		Map<String, Object> matchResult = new HashMap<String, Object>();
		matchResult.put("tournamentID", tournamentId);
		matchResult.put("matchID", matchId);
		matchResult.put("score", score);

		return PropellerSDK.instance().launchWithMatchResult(matchResult, this);
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
	 * @param data Match data returned by the Propeller SDK. The data contains a
	 *        'tournamentID' (String), 'matchID' (String), and 'params'
	 *        (Map<String, Object>) which is a map of game context match
	 *        parameters. The match parameters, at a minimum, will contain a
	 *        match 'seed' (Long) and a match 'round' (Integer) which can be
	 *        used for setting up deterministic properties of the game. If
	 *        configured, the match parameters will also contain match 'options'
	 *        (Map<String, Object>) which is the set of game options selected by
	 *        the match creator.
	 */
	@Override
	public void sdkCompletedWithMatch(Map<String, Object> data) {
		String tournamentId = (String) data.get("tournamentID");
		String matchId = (String) data.get("matchID");

		@SuppressWarnings("unchecked")
		Map<String, Object> params = (Map<String, Object>) data.get("params");

		Long seed = (Long) params.get("seed");
		Integer round = (Integer) params.get("round");

		@SuppressWarnings("unchecked")
		Map<String, Object> options = (Map<String, Object>) params.get("options");

		Integer gameType = (Integer) options.get("gametype");

		nativeSdkCompletedWithMatch(tournamentId, matchId, seed, round, gameType);
	}

	/***************************************************************************
	 * Called when the Propeller SDK failed.
	 * 
	 * @param message Failure message.
	 * @param data Failure data returned by the Propeller SDK.
	 */
	@Override
	public void sdkFailed(String message, Map<String, Object> data) {
		nativeSdkFailed(message, data);
	}

	/***************************************************************************
	 * Called when the Propeller SDK wants to perform a social login.
	 * 
	 * @param context Calling context.
	 * @param allowCache True if cached login credentials can be used, false
	 *        otherwise.
	 * @return True if the social login was performed, false otherwise.
	 */
	@Override
	public boolean sdkSocialLogin(Context context, boolean allowCache) {
		return nativeSdkSocialLogin(allowCache);
	}

	/***************************************************************************
	 * Called when the Propeller SDK wants to perform a social invite.
	 * 
	 * @param context Calling context.
	 * @param subject The subject string for the invite.
	 * @param longMessage The message to invite with (long version).
	 * @param shortMessage The message to invite with (short version).
	 * @param linkUrl The URL for where the game can be obtained.
	 * @return True if the social invite was performed, false otherwise.
	 */
	@Override
	public boolean sdkSocialInvite(Context context, String subject, String longMessage, String shortMessage, String linkUrl) {
		return nativeSdkSocialInvite(subject, longMessage, shortMessage, linkUrl);
	}

	/***************************************************************************
	 * Called when the Propeller SDK wants to perform a social share.
	 * 
	 * @param context Calling context.
	 * @param subject The subject string for the share.
	 * @param longMessage The message to share with (long version).
	 * @param shortMessage The message to share with (short version).
	 * @param linkUrl The URL for where the game can be obtained.
	 * @return True if the social share was performed, false otherwise.
	 */
	@Override
	public boolean sdkSocialShare(Context context, String subject, String longMessage, String shortMessage, String linkUrl) {
		return nativeSdkSocialShare(subject, longMessage, shortMessage, linkUrl);
	}

	/***************************************************************************
	 * Notifies the Propeller SDK that the requested social login has completed.
	 * 
	 * @param result Social login result.
	 * @return True if the result was handled, false otherwise.
	 */
	public boolean sdkSocialLoginCompleted(String result) {
		return PropellerSDK.instance().sdkSocialLoginCompleted(result);
	}

	/***************************************************************************
	 * Notifies the Propeller SDK that the requested social invite has
	 * completed.
	 * 
	 * @return True if the result was handled, false otherwise.
	 */
	public boolean sdkSocialInviteCompleted() {
		return PropellerSDK.instance().sdkSocialInviteCompleted();
	}

	/***************************************************************************
	 * Notifies the Propeller SDK that the requested social share has completed.
	 * 
	 * @return True if the result was handled, false otherwise.
	 */
	public boolean sdkSocialShareCompleted() {
		return PropellerSDK.instance().sdkSocialShareCompleted();
	}

	/***************************************************************************
	 * Sync the current users challenge counts.
	 */
	public void syncChallengeCounts() {
		PropellerSDK.instance().syncChallengeCounts();
	}

	/***************************************************************************
	 * Sync the tournament information.
	 */
	public void syncTournamentInfo() {
		PropellerSDK.instance().syncTournamentInfo();
	}

	/***************************************************************************
	 * Sync the virtual goods.
	 * 
	 * @return True if the virtual goods synchronization request was made, false
	 *         otherwise.
	 */
	public boolean syncVirtualGoods() {
		return PropellerSDK.instance().syncVirtualGoods();
	}

	/***************************************************************************
	 * Acknowledge the virtual goods.
	 * 
	 * @param transactionId Transaction ID of the virtual goods to acknlowledge.
	 * @param consumed Flags whether or not the virtual goods being acknowledged
	 *        were consumed, false otherwise.
	 * @return True if the virtual good acknowledgment request was made, false
	 *         otherwise.
	 */
	public boolean acknowledgeVirtualGoods(String transactionId, boolean consumed) {
		return PropellerSDK.instance().acknowledgeVirtualGoods(transactionId, consumed);
	}

	/***************************************************************************
	 * Notifies the native libraries to deallocate global references in the JNI
	 * bridge.
	 */
	public native void deallocateJNIBridge();

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
	 * @param data Failure data returned by the Propeller SDK.
	 */
	public native void nativeSdkFailed(String message, Map<String, Object> data);

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK social login event
	 * occurred.
	 * 
	 * @param allowCache Flags whether or not using cached login credentials can
	 *        be used to login the user. If allowCache is false then cached
	 *        login credentials should not be used (e.g. switching users)
	 */
	public native boolean nativeSdkSocialLogin(boolean allowCache);

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK social invite event
	 * occurred.
	 * 
	 * @param subject The subject string for the invite.
	 * @param longMessage The message to invite with (long version).
	 * @param shortMessage The message to invite with (short version).
	 * @param linkUrl The URL for where the game can be obtained.
	 */
	public native boolean nativeSdkSocialInvite(String subject, String longMessage, String shortMessage, String linkUrl);

	/***************************************************************************
	 * Notifies the native libraries that the Propeller SDK social share event
	 * occurred.
	 * 
	 * @param subject The subject string for the share.
	 * @param longMessage The message to share with (long version).
	 * @param shortMessage The message to share with (short version).
	 * @param linkUrl The URL for where the game can be obtained.
	 */
	public native boolean nativeSdkSocialShare(String subject, String longMessage, String shortMessage, String linkUrl);

	/***************************************************************************
	 * Notifies the native libraries that the challenge count has been updated.
	 * 
	 * @param count Updated challenge count.
	 */
	public static native void nativeUpdatedChallengeCount(int count);

	/***************************************************************************
	 * Notifies the native libraries that the tournament information has been
	 * updated.
	 * 
	 * @param name Tournament name.
	 * @param campaignName Tournament campaign name.
	 * @param sponsorName Tournament sponsor name.
	 * @param startDate Tournament start date, in seconds.
	 * @param endDate Tournament end date, in seconds.
	 * @param logo Tournament logo.
	 */
	public static native void nativeUpdatedTournamentInfo(String name, String campaignName, String sponsorName, long startDate, long endDate, String logo);

	/***************************************************************************
	 * Notifies the native libraries that the virtual goods have been updated.
	 * 
	 * @param transactionId transaction ID of the virtual goods being updated.
	 * @param virtualGoods Updated list of the virtual goods being updated.
	 */
	public static native void nativeUpdatedVirtualGoods(String transactionId, Object[] virtualGoods);

	/***************************************************************************
	 * Notifies the native libraries that the virtual goods have been updated.
	 * 
	 * @param transactionId transaction ID of the virtual goods being rolled
	 *        back.
	 * @param virtualGoods Updated list of the virtual goods being rolled back.
	 */
	public static native void nativeRollbackVirtualGoods(String transactionId);

}
