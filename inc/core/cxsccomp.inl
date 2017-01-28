// -*-c++-*-
template<typename T>
int Ub(const T & v)
{
  return num_rows(v);
}

template<typename T>
int Lb(const T & )
{
  return 0;
}

template<typename T>
int Lb(const T & , int )
{
  return 0;
}

template<typename T>
int Ub(const T & v, int i)
{
  if(i == 1) return num_rows(v); 
  else   return num_cols(v);
  
}

template<typename T>
T Id(const T & v) 
{
  ArithTraits<mreal>::mat_t m(num_rows(v),num_cols(v));
  for(unsigned i = 0; i < num_rows(v); i++){
    for(unsigned j = 0; j< num_cols(v); j++){
      m[i][j] =  (i==j) ? 1.0 : 0.0;
    }
  }
  return m;
}

