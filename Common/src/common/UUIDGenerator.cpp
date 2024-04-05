#include <common\UUIDGenerator.h>

#include <common\uuid.h>

tstring UUIDGenerator::Generate()
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size>{};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);
	uuids::uuid_random_generator gen{ generator };

	const uuids::uuid id = gen();
	
	const tstring uuidString = uuids::to_string<wchar_t>(id);

	return uuidString;
}
