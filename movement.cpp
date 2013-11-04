//movement
//Deals with the movement acroos the debris_field

#define HEAD_DIST 3
#define SIDE_DIST 4

#ifdef DEBUG_ACTIVE
float movement_last_debris;
float movement_distance;
#endif

	float delta[3];
	float head[3];

	mathVecSubtract(delta, dst, &our_state[POS], 3);		//delta = dst - pos
	mathVecScalarMult(head, delta, 1.0f/mathVecMagnitude(delta, 3), 3);  //head = delta / |delta|

	if(mathVecMagnitude(delta, 3) < MAX_ITEM_START_DIST && mathVecMagnitude(&our_state[VEL], 3) < MAX_ITEM_START_VEL) {
		return true;
	} else {
		//delta = zero
		delta[0] = delta[1] = delta[2] = 0.0f;
		api.setAttRateTarget(delta);
	}
fdgfgfgta, 3);

	const float danger_radius = (SPHERE_RADIUS + DEBRIS_RADIUS) + 0.03f;
	const float correction = danger_radius + 0.02f;

	if(direct || seconds >= 90) {
		api.setPositionTarget(dst);
		return false;
	}

	int debris_number;
	float debris_vector[5];
	int nearest_debris = -1;
	float nearest_debris_distance = 1000000.0f;
	float nearest_debris_vector[5];
	for(debris_number = 0; debris_number < NUMBER_OF_DEBRIS; debris_number++) {
		if(!is_debris_collected[debris_number]) {
			distanceToDebris(&our_state[POS], dst, debris_position[debris_number], debris_vector);
			if((debris_vector[SIDE_DIST] - (danger_radius + ((mathVecInner(side_vel, debris_vector, 3)/head_speed)*debris_vector[3]))) < 0.0f) {
				//Colisión
				if(debris_vector[HEAD_DIST] < nearest_debris_distance && debris_vector[HEAD_DIST] > 0.0f) {
					nearest_debris = debris_number;
					int i;
					for(i = 0; i < 5; ++i) {
						nearest_debris_vector[i] = debris_vector[i];
					}
					nearest_debris_distance = debris_vector[3];
				}
			}
		}
	}

	api.setPositionTarget(dst);
	if(nearest_debris >= 0) { //Si nearest_debris es un debris válido
		//delta = tmp
		float needed_speed_up = correction - (nearest_debris_vector[SIDE_DIST] + ((mathVecInner(side_vel, nearest_debris_vector, 3)/head_speed)*nearest_debris_vector[3]));
		mathVecScalarMult(delta, nearest_debris_vector, needed_speed_up, 3);

		api.setForces(delta);
	}
	#ifdef DEBUG_ACTIVE
	else {
		movement_last_debris = -1.0f;
		nearest_debris_distance = 0.0f;
		DEBUG(("movement:Clear!\n"));
	}
	#endif

	return false;
}
