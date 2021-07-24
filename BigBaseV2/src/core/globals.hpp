#pragma once
#include "data/player_struct.hpp"
#include "enums.hpp"

#ifndef GLOBALS_H
#define GLOBALS_H

using namespace big;
struct globals {
	nlohmann::json default_options;
	nlohmann::json options;

	struct tunables {
		bool disable_phone = false;
		bool no_idle_kick = false;
	};

	struct player {
		int character_slot = 1;
		int set_level = 130;
		bool spectating = false;
	};

	struct self {
		bool godmode = false;
		bool off_radar = false;
		bool noclip = false;
		bool no_ragdoll = false;
	};

	struct vehicle {
		bool horn_boost = false;
		SpeedoMeter speedo_meter = SpeedoMeter::DISABLED;
	};

	struct weapons {
		CustomWeapon custom_weapon = CustomWeapon::NONE;
		char vehicle_gun_model[12] = "bus";
	};

	struct window {
		bool main = true;
		bool log = false;
		bool users = true;
		bool player = false;

		int x;
		int y;
	};

	int friend_count = 0;
	int player_count = 0;
	CPlayer players[32];
	CPlayer selected_player;

	tunables tunables{};
	player player{};
	self self{};
	vehicle vehicle{};
	weapons weapons{};
	window window{};

	void from_json(const nlohmann::json& j)
	{
		this->tunables.disable_phone = j["tunables"]["disable_phone"];
		this->tunables.no_idle_kick = j["tunables"]["no_idle_kick"];

		this->self.godmode = j["self"]["godmode"];
		this->self.off_radar = j["self"]["off_radar"];
		this->self.no_ragdoll = j["self"]["no_ragdoll"];

		this->vehicle.horn_boost = j["vehicle"]["horn_boost"];
		this->vehicle.speedo_meter = (SpeedoMeter)j["vehicle"]["speedo_meter"];

		this->weapons.custom_weapon = (CustomWeapon)j["weapons"]["custom_weapon"];

		this->window.log = j["window"]["log"];
		this->window.main = j["window"]["main"];
		this->window.users = j["window"]["users"];
	}

	nlohmann::json to_json()
	{
		return nlohmann::json{
			{
				"tunables", {
					{ "disable_phone", this->tunables.disable_phone },
					{ "no_idle_kick", this->tunables.no_idle_kick }
				}
			},
			{
				"self", {
					{ "godmode", this->self.godmode },
					{ "off_radar", this->self.off_radar },
					{ "no_ragdoll", this->self.no_ragdoll }
				}
			},
			{
				"vehicle", {
					{ "horn_boost", this->vehicle.horn_boost },
					{ "speedo_meter", (int)this->vehicle.speedo_meter }
				}
			},
			{
				"weapons", {
					{ "custom_weapon", (int)this->weapons.custom_weapon }
				}
			},
			{
				"window", {
					{ "log", this->window.log },
					{ "main", this->window.main },
					{ "users", this->window.users }
				}
			}
		};
	}

	void attempt_save()
	{
		nlohmann::json& j = this->to_json();
		if (j != this->options)
		{
			this->save();

			this->options = j;
		}
	}

	bool load()
	{
		this->default_options = this->to_json();

		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ifstream file(settings_file);

		if (!file.is_open())
		{
			this->write_default_config();

			file.open(settings_file);
		}

		file >> this->options;

		bool should_save = this->deep_compare(this->options, this->default_options);

		this->from_json(this->options);

		if (should_save)
		{
			LOG(INFO) << "Updating settings.";
			save();
		}

		return true;
	}

private:
	bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings)
	{
		bool should_save = false;

		for (auto& e : default_settings.items())
		{
			const std::string &key = e.key();

			if (current_settings.count(key) == 0)
			{
				current_settings[key] = e.value();

				should_save = true;
			}
			else if (current_settings[key].is_structured() && e.value().is_structured())
			{
				if (deep_compare(current_settings[key], e.value()))
					should_save = true;
			}
			else if (current_settings[key].type() != e.value().type()) {
				current_settings[key] = e.value();

				should_save = true;
			}
		}

		return should_save;
	}

	bool save()
	{
		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
		file << this->to_json().dump(4);
		file.close();

		return true;
	}

	bool write_default_config()
	{
		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ofstream file(settings_file, std::ios::out);
		file << this->to_json().dump(4);
		file.close();

		return true;
	}
};

inline struct globals g;
#endif // !GLOBALS_H