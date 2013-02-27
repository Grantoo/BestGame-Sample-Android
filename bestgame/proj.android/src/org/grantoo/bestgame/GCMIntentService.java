/*
 * Copyright 2012 Google Inc. Licensed under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law
 * or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
package org.grantoo.bestgame;

import static org.grantoo.bestgame.bestgame.SENDER_ID;
import org.grantoo.lib.propeller.PropellerSDK;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import com.google.android.gcm.GCMBaseIntentService;

/**
 * IntentService responsible for handling GCM messages.
 */
public class GCMIntentService extends GCMBaseIntentService {

	/***************************************************************************
	 * Constructor.
	 */
	public GCMIntentService() {
		super(SENDER_ID);
	}

	/***************************************************************************
	 * Called after a device has been registered.
	 * 
	 * @param context Application's context.
	 * @param registrationId The registration ID returned by the GCM service.
	 */
	@Override
	protected void onRegistered(Context context, String registrationId) {
		PropellerSDK.setNotificationToken(registrationId);
	}

	/***************************************************************************
	 * Called after a device has been unregistered.
	 * 
	 * @param context Application's context.
	 * @param registrationId The registration ID that was previously registered.
	 */
	@Override
	protected void onUnregistered(Context context, String registrationId) {
		PropellerSDK.setNotificationToken(null);
	}

	/***************************************************************************
	 * Called when a cloud message has been received.
	 * 
	 * @param context Application's context.
	 * @param intent Intent containing the message payload as extras.
	 */
	@Override
	protected void onMessage(Context context, Intent intent) {
		PropellerSDK.handleGCMNotification(
			context,
			intent,
			new Intent(
				context,
				NotificationActivity.class),
			R.drawable.icon,
			BitmapFactory.decodeResource(
				getResources(),
				R.drawable.icon));
	}

	/***************************************************************************
	 * Called on registration or unregistration error.
	 * 
	 * @param context Application's context.
	 * @param errorId Error id returned by the GCM service.
	 */
	@Override
	public void onError(Context context, String errorId) {
	}

}