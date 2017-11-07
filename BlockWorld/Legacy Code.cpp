/*
void generateChunkAO(GameState *state, Chunk* chunk)
{
	chunk->mesh.lightValues.clear();
	for (std::unordered_map<uint32_t, Block>::iterator iter = chunk->blocks.begin(); iter != chunk->blocks.end(); ++iter)
	{

		glm::ivec3 position = intToVec3(iter->first);

		auto blockIter = chunk->blocks.find(vec3ToInt(position));
		if (blockIter != chunk->blocks.end())
		{
			Block block = blockIter->second;


			//@SPEED I may be able to reduce how many of these I need for calculating vertex AO
			bool leftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y, position.z))) == 0);
			bool rightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y, position.z))) == 0);
			bool upEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y + 1, position.z))) == 0);
			bool downEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y - 1, position.z))) == 0);
			bool frontEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y, position.z + 1))) == 0);
			bool backEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y, position.z - 1))) == 0);

			bool backTopLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y + 1, position.z - 1))) == 0);
			bool backTopMidEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y + 1, position.z - 1))) == 0);
			bool backTopRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y + 1, position.z - 1))) == 0);
			bool backMidLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y, position.z - 1))) == 0);
			bool backMidRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y, position.z - 1))) == 0);
			bool backBottomLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y - 1, position.z - 1))) == 0);
			bool backBottomMidEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y - 1, position.z - 1))) == 0);
			bool backBottomRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y - 1, position.z - 1))) == 0);

			bool frontTopLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y + 1, position.z + 1))) == 0);
			bool frontTopMidEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y + 1, position.z + 1))) == 0);
			bool frontTopRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y + 1, position.z + 1))) == 0);
			bool frontMidLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y, position.z + 1))) == 0);
			bool frontMidRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y, position.z + 1))) == 0);
			bool frontBottomLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y - 1, position.z + 1))) == 0);
			bool frontBottomMidEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y - 1, position.z + 1))) == 0);
			bool frontBottomRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y - 1, position.z + 1))) == 0);

			bool midTopLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y + 1, position.z))) == 0);
			bool midTopRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y + 1, position.z))) == 0);
			bool midBottomLeftEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y - 1, position.z))) == 0);
			bool midBottomRightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y - 1, position.z))) == 0);



			if (upEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!backTopMidEmpty, (int)!backTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!frontTopMidEmpty, (int)!frontTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!frontTopMidEmpty, (int)!midTopRightEmpty, (int)!frontTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!backTopMidEmpty, (int)!backTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!frontTopMidEmpty, (int)!midTopRightEmpty, (int)!frontTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backTopMidEmpty, (int)!midTopRightEmpty, (int)!backTopRightEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Top, &chunk->mesh.uvs);
			}

			if (downEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomLeftEmpty, (int)!backBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomRightEmpty, (int)!backBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomRightEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomLeftEmpty, (int)!backBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomRightEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomLeftEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomLeftEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Bottom, &chunk->mesh.uvs);
			}

			if (leftEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomLeftEmpty, (int)!backBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomLeftEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!frontTopMidEmpty, (int)!frontTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomLeftEmpty, (int)!backBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!frontTopMidEmpty, (int)!frontTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!backTopMidEmpty, (int)!backTopLeftEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Left, &chunk->mesh.uvs);
			}

			if (rightEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomRightEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomRightEmpty, (int)!backBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backTopMidEmpty, (int)!midTopRightEmpty, (int)!backTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomRightEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backTopMidEmpty, (int)!midTopRightEmpty, (int)!backTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!frontTopMidEmpty, (int)!midTopRightEmpty, (int)!frontTopRightEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Right, &chunk->mesh.uvs);
			}

			if (frontEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomLeftEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomRightEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!frontTopMidEmpty, (int)!midTopRightEmpty, (int)!frontTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midBottomLeftEmpty, (int)!frontBottomMidEmpty, (int)!frontBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!frontTopMidEmpty, (int)!midTopRightEmpty, (int)!frontTopRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!frontTopMidEmpty, (int)!frontTopLeftEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Front, &chunk->mesh.uvs);
			}

			if (backEmpty)
			{
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomRightEmpty, (int)!backBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomLeftEmpty, (int)!backBottomLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!backTopMidEmpty, (int)!backTopLeftEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backBottomMidEmpty, (int)!midBottomRightEmpty, (int)!backBottomRightEmpty) + MIN_LIGHT_VAL);
				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!midTopLeftEmpty, (int)!backTopMidEmpty, (int)!backTopLeftEmpty) + MIN_LIGHT_VAL);

				chunk->mesh.lightValues.push_back(AO_SCALE * calcVertexAO((int)!backTopMidEmpty, (int)!midTopRightEmpty, (int)!backTopRightEmpty) + MIN_LIGHT_VAL);

				addUVs((BlockID)block.id, Back, &chunk->mesh.uvs);
			}

		}
	}
	chunk->mesh.model = loadToVAO(&chunk->mesh.vertices[0], (unsigned int)chunk->mesh.vertices.size(), &chunk->mesh.uvs[0],
		(unsigned int)chunk->mesh.uvs.size(), &chunk->mesh.lightValues[0],
		(unsigned int)chunk->mesh.lightValues.size());
}
*/