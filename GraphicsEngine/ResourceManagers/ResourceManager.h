#pragma once
#include<map>
#include<string>
#include<memory>

/**
	Base class used to implement common functions used by resource managers.
*/
template <class T>
class ResourceManager
{
public:
	/**
		Destructor.
	*/
	virtual ~ResourceManager(){	}
protected:
	/**
		Adds a resource to the container.
		@param name resource's name.
		@param resource resource to add to the container.
	*/
	void add(const std::string& name, std::shared_ptr<T> resource);
	/**
		Finds a resource in the container.
		@param name name of the resource we want to find.
		@return pointer to the resource with a given name. Null if not found.
	*/
	std::shared_ptr<T> find(const std::string& name) const;
	/**
		Removes resource from the container.
		@param name name of the resource we want to remove.
	*/
	void remove(const std::string& name);
	/**
		Extracts a name from a given filename.
		@param filename out of which to extract a name.
		@return extracted name.
	*/
	std::string extractName(const std::string& filename) const;
#ifdef _DEBUG
	uint64_t hit = 0;									//*< Records the number of resources that did not need to be loaded from disk.
	uint64_t miss = 0;									//*< Records the number of resources that needed to be loaded from disk.
#endif
private:
	std::map<std::string, std::shared_ptr<T>> resources;	//*< Container used for storing resources.
};

template<class T>
inline void ResourceManager<T>::add(const std::string & name, std::shared_ptr<T> resource)
{
	std::pair<std::string, std::shared_ptr<T>> record{name, resource};
	resources.insert(record);
}

template<class T>
inline std::shared_ptr<T> ResourceManager<T>::find(const std::string & name) const
{
	typename std::map<std::string, std::shared_ptr<T>>::const_iterator it;
	it = resources.find(name);
	if (it != resources.end())
	{
		return (*it).second;
	}
	return nullptr;
}

template<class T>
inline void ResourceManager<T>::remove(const std::string & name)
{
	typename map<const std::string, T*>::iterator it = resources.find(name);
	if (it != resources.end())
	{
		resources.erase(it);
	}
}

template<class T>
inline std::string ResourceManager<T>::extractName(const std::string & filename) const
{
	size_t offset = filename.find_last_of('/');
	if (offset == std::string::npos)
	{
		offset = filename.find_last_of('\\');
		if (offset == std::string::npos)
		{
			offset = 0;
		}
	}
	size_t offsetDot = filename.find_last_of('.');
	//Take a substring between last '/' and '.', and treat it as a name of a resource;
	std::string name = filename.substr(offset + 1, offsetDot - offset - 1);
	return name;
}
