#pragma once
#include "common.hpp"

namespace big
{
	class looped {
	public:
		static void tunables_disable_phone();
		static void tunables_no_idle_kick();

		static void player_specate();

		static void self_godmode();
		static void self_off_radar();
		static void self_noclip();
		static void self_no_ragdoll();

		static void system_update_players();
		static void system_screen_size();

		static void weapons_cage_gun();
		static void weapons_delete_gun();
		static void weapons_gravity_gun();
		static void weapons_repair_gun();
		static void weapons_steal_vehicle_gun();
		static void weapons_vehicle_gun();

		static void vehicle_horn_boost();
		static void vehicle_speedo_meter();
	};
}