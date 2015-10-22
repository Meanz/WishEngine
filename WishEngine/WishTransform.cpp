
namespace Wish
{

	wish_transform::wish_transform(const char* name)
	{
		this->Scale = vec3(1.0f);
		this->Rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
		this->Local = mat4(1.0f);
		this->Global = mat4(1.0f);
		this->Child = NULL;
		this->Next = NULL;
		this->Parent = NULL;
		this->TransformType = wish_transform_type::WISH_TRANSFORM_TYPE_TRANSFORM;
		SetName(name);
	}

	wish_transform::wish_transform() : wish_transform("Null") {}

	wish_transform::~wish_transform()
	{
		//Detatch this thing if it exists
		if (Parent != NULL)
		{
			//Attach all children to my parent?
			if (Child != NULL)
			{
				wish_transform* child = Child;
				while (child != NULL)
				{
					//TODO(Meanzie): Attach these to my parent?
					Detach(child);
					child = child->Next;
				}
			}
			Parent->Detach(this);
		}
		else
		{
			//Parent == null? 
			//TODO(Meanzie): Handle this case
		}
	}

	b32 wish_transform::HasChild(wish_transform* child)
	{
		bool result = false;
		wish_transform* firstChild = Child;
		if (firstChild)
		{
			while (firstChild != NULL)
			{
				if (firstChild == child) {
					result = true;
					break;
				}
				firstChild = firstChild->Next;
			}
		}
		return result;
	}

	void wish_transform::SetName(const char* name)
	{
		if (strlen(name) < WISH_TRANSFORM_NAME_SIZE) {
			strcpy_s(Name, name);
		}
	}

	wish_transform* wish_transform::GetLastChild()
	{
		if (Child)
		{
			wish_transform* it = Child;
			while (it->Next != NULL)
			{
				it = it->Next;
			}
			return it;
		}
		return NULL;
	}

	void wish_transform::Attach(wish_transform* other)
	{
		//Attach us to the last child
		if (!Child)
		{
			//We are inserting the first transform
			other->Parent = this;
			other->Prev = NULL;
			other->Next = NULL;
			Child = other;
		}
		else
		{
			wish_transform* lastChild = GetLastChild();
			if (lastChild) {
				lastChild->Next = other;
				other->Parent = lastChild->Parent;
				other->Next = NULL;
				other->Prev = lastChild;
			}
			else {
				//Something weeeeird is happening
				throw runtime_error("Could not retrieve last child, broken hierarchy?");
			}
		}
	}

	void wish_transform::Detach(wish_transform* other)
	{
		if (HasChild(other))
		{
			if (other->Next != NULL)
			{
				other->Next->Prev = other->Prev;
			}
			if (other->Prev != NULL)
			{
				other->Prev->Next = other->Next;
			}
			other->Prev = NULL;
			other->Next = NULL;
			if (Child == other)
			{
				Child = NULL;
			}
		}
		else
		{
			printf("%s:%i Tried to remove child, but supplied parent didn't have the child.", __FUNCTION__, __LINE__);
		}
	}

	mat4& wish_transform::CalculateLocal()
	{
		Local = mat4_cast(Rotation) * glm::translate(mat4(1.0f), Position) * glm::scale(mat4(1.0f), Scale);
		return Local;
	}

	mat4& wish_transform::CalculateGlobal(mat4& parent)
	{
		CalculateLocal();
		Global = parent * Local;
		return Global;
	}

	void wish_transform::LookAt(v3 eye, v3 center, v3 up)
	{
		mat4 lookat_matrix = glm::lookAt(eye, center, up);
		this->Rotation = quat(lookat_matrix);
		this->Position = -eye;
	}

}

b32 Wish::Wish_Physics_AABB_AABB(wish_aabb* a1, wish_aabb* a2) {
	return
		a1->Min.x < a2->Max.x &&
		a1->Max.x > a2->Min.x &&
		a1->Min.y < a2->Max.y &&
		a1->Max.y > a2->Min.y &&
		a1->Min.z < a2->Max.z &&
		a1->Max.z > a2->Min.z;
}

/*
void Transform::Rotate(float angle, vec3 axis) {
quat angAx = glm::angleAxis(glm::radians(angle), axis);
m_Rotation = m_Rotation * angAx;
}

void Transform::Translate(vec3 other) {
m_Position += other;
}

void Transform::Translate(float x, float y, float z) {
m_Position.x += x;
m_Position.y += y;
m_Position.z += z;
}

void Transform::LookAt(vec3 eye, vec3 center) {
glm::mat4 lookat_matrix = glm::lookAt(eye, center, vec3(0.0f, 1.0f, 0.0f));
m_Rotation = quat(lookat_matrix);
m_Position = -eye;
}*/