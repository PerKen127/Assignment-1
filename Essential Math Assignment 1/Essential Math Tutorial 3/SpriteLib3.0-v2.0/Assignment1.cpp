#include "Assignment1.h"
#include "Utilities.h"

//creates a timer for the inputs
int jumpTimer = 0;
int runTimer = 0;

Assignment1::Assignment1(std::string name)
{
	//gravity
	m_gravity = b2Vec2(0.f, -300.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void Assignment1::InitScene(float windowWidth, float windowHeight)
{
	//dynamically allocates the register
	m_sceneReg = new entt::registry;

	//attach the register
	ECS::AttachRegister(m_sceneReg);

	//sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//setup maincamera Entity
	{
		//creates camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-200.f, 200.f, -200.f, 200.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//setup new Entity
	{
		//creates entity
		auto entity = ECS::CreateEntity();

		//add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//set up the components
		std::string fileName = "BackgroundPlaceholder.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2000, 2000);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//player entity
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//sets up the components
		std::string fileName = "PlayerPlaceholder.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));
		tempDef.angle = Transform::ToRadians(0.f);

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetFixedRotation(true);
	}

	//setup ground platform
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "GroundPlaceholder.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2000, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	//setup ground platform
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "GroundPlaceholder.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 75, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(35.f), float32(20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);

	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void Assignment1::Update()
{
	Scene::AdjustScrollOffset();
}

void Assignment1::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float speed = 10.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	

	if (Input::GetKey(Key::Shift))
	{
		speed *= 7.f;
	}

	if (Input::GetKey(Key::W) || Input::GetKey(Key::UpArrow))
	{
		jumpTimer++;
		
		vel += b2Vec2(0.f, 1.f);

		if (jumpTimer >= 650)
		{
			vel += b2Vec2(0.f, -1.f);
		}
		
	}

	if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow))
	{
		for (int i = 0; i < 1000; i++)
		{
			if (i == 0 || i % 100 == 0)
			{
				vel += b2Vec2(-0.1f, 0.f);
			}
		}
	}

	if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow))
	{
		for (int i = 0; i < 1000; i++)
		{
			if (i == 0 || i % 100 == 0)
			{
				vel += b2Vec2(0.1f, 0.f);
			}
		}
	}

	player.GetBody()->SetLinearVelocity(speed * vel);
}

void Assignment1::KeyboardDown()
{

}

void Assignment1::KeyboardUp()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float speed = 10.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKeyUp(Key::W) || Input::GetKeyUp(Key::UpArrow))
	{
		jumpTimer = 0;
		runTimer = 0;
	}

	if (Input::GetKeyUp(Key::A) || Input::GetKeyUp(Key::LeftArrow))
	{
		jumpTimer = 0;
		runTimer = 0;
	}

	if (Input::GetKeyUp(Key::D) || Input::GetKeyUp(Key::RightArrow))
	{
		jumpTimer = 0;
		runTimer = 0;
	}
}