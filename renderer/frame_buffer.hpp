#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "pixel_sampler.hpp"

namespace sr {

using uint = unsigned int;

class FrameBuffer final {
public:
	typedef std::shared_ptr<FrameBuffer> ptr;

	// ctor/dtor.
	FrameBuffer(int width, int height);
	~FrameBuffer() = default;

	void clearDepth(const float &depth);
	void clearColor(const glm::vec4 &color);
	void clearColorAndDepth(const glm::vec4 &color, const float &depth);

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	const DepthBuffer &getDepthBuffer() const { return m_depthBuffer; }
	const ColorBuffer &getColorBuffer() const { return m_colorBuffer; }

	float readDepth(const uint &x, const uint &y, const uint &i) const;
	PixelRGBA readColor(const uint &x, const uint &y, const uint &i) const;

	void writeDepth(const uint &x, const uint &y, const uint &i, const float &value);
	void writeColor(const uint &x, const uint &y, const uint &i, const glm::vec4 &color);
	void writeColorWithMask(const uint &x, const uint &y, const glm::vec4 &color, const MaskPixelSampler &mask);
	void writeColorWithMaskAlphaBlending(const uint &x, const uint &y, const glm::vec4 &color, const MaskPixelSampler &mask);
	void writeDepthWithMask(const uint &x, const uint &y, const DepthPixelSampler &depth, const MaskPixelSampler &mask);

	// MSAA 
	const ColorBuffer &resolve();

private:
	DepthBuffer m_depthBuffer;
	ColorBuffer m_colorBuffer;
	unsigned int m_width, m_height;
	
};

} // namespace sr