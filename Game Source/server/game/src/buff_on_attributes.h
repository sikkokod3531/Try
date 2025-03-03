#pragma once
class CHARACTER;

class CBuffOnAttributes
{
public:
	CBuffOnAttributes(LPCHARACTER pOwner, BYTE m_point_type, std::vector <BYTE>* vec_buff_targets);
	~CBuffOnAttributes();

	void RemoveBuffFromItem(LPITEM pItem);
	void AddBuffFromItem(LPITEM pItem);
	void ChangeBuffValue(BYTE bNewValue);

	bool On(BYTE bValue);
	void Off();

	void Initialize();
private:
	LPCHARACTER m_pBuffOwner;
	BYTE m_bPointType;
	BYTE m_bBuffValue;
	std::vector <BYTE>* m_p_vec_buff_wear_targets;

	// apply_type, apply_value
	typedef std::map <BYTE, int> TMapAttr;
	TMapAttr m_map_additional_attrs;
};
