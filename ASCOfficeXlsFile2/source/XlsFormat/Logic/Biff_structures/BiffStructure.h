#pragma once

#include <vector>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include <Auxiliary/BetterVariantT.h>
#include <Logging/Log.h>

//#include <Logic/Biff_structures/BitMarkedStructs.h>

class Document;

namespace XLS
{;

class CFRecord;


class BiffStructure_NoVtbl
{
public:
	void load(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))
	void store(CFRecord& record); // this function will never be called ( look at operator>>(CFRecord& record, T& val))

// protected:
// 	static MSXML2::IXMLDOMElementPtr createElement(BiffStructurePtr & parent, const std::wstring  tag_name);
};

class BiffStructure;
typedef boost::shared_ptr<BiffStructure>	BiffStructurePtr;
typedef std::vector<BiffStructurePtr>		BiffStructurePtrVector;

class BiffStructure : protected BiffStructure_NoVtbl
{
public:

	virtual BiffStructurePtr clone() = 0;

	Document*	get_document();
	void		set_document(Document* d);

	//// Makes XML data
	////virtual void toXML(BiffStructurePtr & parent);
	//// Reads XML data
	////virtual const bool fromXML(BiffStructurePtr & parent, const size_t position, const bool is_mandatory);

	////virtual const bool fromXML(BiffStructurePtr & parent);

	virtual void load(CFRecord& record) = 0;
	virtual void store(CFRecord& record) = 0;

	virtual int serialize(std::wostream & _stream)
	{
		std::wstringstream s;
		s << std::wstring(L"This element - ") << getClassName() << std::wstring(L"- not serialize");
		Log::warning(s.str());
		return 0;
	}

	virtual const std::wstring & getClassName() const = 0;   // Must be overridden in every deriver. The return value must be a reference to a static variable inside the getter

	std::wstring tagName_;
protected:
	Document * document_;
	//const AUX::BetterVariantT getStructAttribute(BiffStructurePtr & parent, const std::wstring & attrib_name);
	//MSXML2::IXMLDOMElementPtr getStructNode(BiffStructurePtr & parent, const std::wstring & tag_name);
};

#define BASE_STRUCTURE_DEFINE_CLASS_NAME(class_name)\
	public: \
		const std::wstring & getClassName() const { static std::wstring  str(L# class_name); return str; };
	


bool DiffBiff(BiffStructure_NoVtbl & val);
bool DiffBiff(BiffStructure & val);


template<class T>
CFRecord& operator>>(CFRecord& record, T& val)
{
	if(DiffBiff(val))
	{
		record.loadAnyData(val);
	}
	else
	{
		val.load(record);
	}
	return record;
}


template<class T>
CFRecord& operator<<(CFRecord& record, T& val)
{
	if(DiffBiff(val))
	{
		record.storeAnyData(val);
	}
	else
	{
		val.store(record);
	}
	return record;
}



} // namespace XLS

