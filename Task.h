#pragma once

#ifndef DEF_EXTENSION_H

#define DEF_EXTENSION_H
#define SUCCESS 0
#define INVALID -1

#endif // !DEF_EXTENSION_H

//Šî’êƒNƒ‰ƒX
class Task
{
public:
	virtual ~Task(){}

	//=0‚Æ‚È‚Á‚Ä‚¢‚é‚à‚Ì‚ÍŒp³æ‚ÅÀ‘•‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢
	//{}‚Æ‚È‚Á‚Ä‚¢‚é‚à‚Ì‚ÍŒp³æ‚ÅÀ‘•‚µ‚È‚­‚Ä‚à‚æ‚¢
	virtual void Initialize(){}
	virtual void Update() = 0;
	virtual void DrawLoop() = 0;
	virtual void Finalize(){}
};

