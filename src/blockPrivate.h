#ifndef _BLOCK_OBJECT_H_
#define _BLOCK_OBJECT_H_

class blockPrivate
{
public:
	enum mineStatus {
		mineNormal = 0x00,
		mineHover = 0x01,
		mineClicked = 0x02,
		mineFlaged = 0x03,
		mineExplode = 0x04
	};

	blockPrivate();
	~blockPrivate();

	// member function 
	void setStatus(mineStatus _status);
	mineStatus getStatus();

	void setSweep(bool _sweep);
	bool getSweeped();

	void setFlag(bool _flag);
	bool getFlag();

	void setNearBy(int _nearBy);
	int getNearBy();

	void setMine(bool _isMine);
	bool getMine();

private:
	bool sweeped;
	bool flaged;
	bool isMine;
	int nearBy;
	mineStatus status;
};

#endif // _BLOCK_OBJECT_H_