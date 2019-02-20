#include "model.h"
#include <iostream>

using namespace LRender;

Model::Model(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
	createBuffers();
	upload(vertices, indices);
}

Model::~Model() {
	freeBuffers();
}

void Model::draw() const {
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void Model::createBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(BUFFER_COUNT, buffers);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[BUFFER_VERTICES]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[BUFFER_INDICES]);
}

void Model::upload(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	count = indices.size();
}

void Model::freeBuffers() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(BUFFER_COUNT, buffers);
}