#pragma once

#ifndef DEF_EXTENSION_H

#define DEF_EXTENSION_H
#define SUCCESS 0
#define INVALID -1

#endif // !DEF_EXTENSION_H

//基底クラス
class Task
{
public:
	virtual ~Task(){}

	//=0となっているものは継承先で実装しなければいけない
	//{}となっているものは継承先で実装しなくてもよい
	virtual void Initialize(){}
	virtual void Update() = 0;
	virtual void DrawLoop() = 0;
	virtual void Finalize(){}
};

