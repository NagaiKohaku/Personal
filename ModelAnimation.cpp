#include "ModelAnimation.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "cassert"

Animation ModelAnimation::LoadAnimationFile(const std::string& filePath) {

	Animation animation;

	Assimp::Importer importer;

	std::string filePath = directoryPath_ + "/" + filePath;

	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	assert(scene->mNumAnimations != 0);

	aiAnimation* animationAssimp = scene->mAnimations[0];

	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {

		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];

		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {

			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];

			KeyframeVector3 keyframe;

			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = Vector3(-keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z);
			nodeAnimation.translate.keyframes.push_back(keyframe);

		}
	}

}