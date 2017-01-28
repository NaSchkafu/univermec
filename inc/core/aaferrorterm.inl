    template<class T>
    class AAFErrorTerm 
    {
    public:
      typedef T base_t;
      
      AAFErrorTerm(const base_t& term);
      
      const T& get() const;
      T& get();

      mreal error() const;

    private:
      T m_term;
    };

    template<typename T>
    bool operator<(const AAFErrorTerm<T>& first, const AAFErrorTerm<T>& second);
    template<typename T>
    bool operator==(const AAFErrorTerm<T>& first, const AAFErrorTerm<T>& second);

    template<typename T>
    bool operator<(const AAFErrorTerm<T>& first, const AAFErrorTerm<T>& second)
    {
      return first.get() < second.get();
    }
    
    template<typename T>
    bool operator==(const AAFErrorTerm<T>& first, const AAFErrorTerm<T>& second)
    {
      return !(first.get() < second.get() || second.get() < first.get());
    }
    

    template<class T>
    AAFErrorTerm<T>::AAFErrorTerm(const base_t& term)
      :m_term(term)
    { }

    template<class T>
    const T& AAFErrorTerm<T>::get() const
    {
      return m_term;
    }

    template<class T>
    T& AAFErrorTerm<T>::get()
    {
      return m_term;
    }

    template<typename T>
    mreal AAFErrorTerm<T>::error() const 
    {
      return m_term.dev();
    }

