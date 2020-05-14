#define DECLARE_NO_COPY_CLASS(classname)        \
    private:                                    \
        classname(const classname&) = delete;            \
        classname& operator=(const classname&) = delete;


#define DECLARE_CONSTR_DESTR_CLASS(classname)        \
    public:                                    \
        classname();            \
        ~classname();


#define DECLARE_DEFAULT_CONSTRUCTOR_CLASS(classname)        \
    public:                                    \
        classname() = default;


#define DECLARE_DESTRUCTOR_CLASS(classname)   \
    public:                                    \
        ~classname();


#define d_cast(T, v) (dynamic_cast<T*>(v)) // usage if pointer is  not null
#define s_cast(T, v) (static_cast<T*>(v())) // usage if pointer is  not null