//-*-c++-*-

template <typename T>
bool FGPUEval<T>::open_device_()
{
  return m_gpu_host->open_device();
}

template <typename T>
void FGPUEval<T>::close_device_()
{
  m_gpu_host->close_device();
}

template <typename T>
IGPUFuture<core::arith::ivector>* FGPUEval<T>::comp_(const std::vector<core::arith::ivector> &args) const
{
  unsigned dim = get_f().dim();
  unsigned size = args.size();
  double *gx = new double[dim*2*size];

  for(unsigned i = 0; i < size; i+=m_bsize) {
    unsigned rem = std::min(size - i, m_bsize);
    unsigned offset = i*dim*2;
    for(unsigned j = 0; j < rem; j++) {
      for(unsigned k = 0; k < 2*dim; k+=2) {
        gx[2*j + k*rem+offset] = to_double(inf(args[j+i][k/2]));
        gx[2*j + k*rem+1+offset] = to_double(sup(args[j+i][k/2]));
      }
    }
  }


  // unsigned dim = get_f().dim();
  
  // unsigned i = 0;
  // BOOST_FOREACH(const core::arith::ivector &x, args) {
  //   assert(size(x) == dim && "Different vector dimensions in input!");
  //   do {
  //     gx[i] = to_double(inf(x[(i/2) % dim]));
  //     gx[i+1] = to_double(sup(x[(i/2) % dim]));
  //     i += 2;
  //   } while(i % (2*dim));
  // }

  return new GPUFutureImpl<core::arith::ivector>(m_gpu_host->comp_iv(gx, args.size()));
}

template <typename T>
IGPUFuture<core::arith::ivector>* FGPUEval<T>::comp_(const std::vector<core::Box*> &args) const
{
  unsigned dim = get_f().dim();
  unsigned size = args.size();
  double *gx = new double[dim*2*size];

  for(unsigned i = 0; i < size; i+=m_bsize) {
    unsigned rem = std::min(size - i, m_bsize);
    unsigned offset = i*dim*2;
    for(unsigned j = 0; j < rem; j++) {
      for(unsigned k = 0; k < 2*dim; k+=2) {
        gx[2*j + k*rem+offset] = to_double(inf(args[j+i]->ref_X()[k/2]));
        gx[2*j + k*rem+1+offset] = to_double(sup(args[j+i]->ref_X()[k/2]));
      }
    }
  }

  // unsigned dim = get_f().dim();
  // double *gx = new double[dim*2*args.size()];
  // unsigned i = 0;
  // BOOST_FOREACH(core::Box* x, args) {
  //   assert(size(x->ref_X()) == dim && "Different vector dimensions in input!");
  //   do {
  //     gx[i] = to_double(inf(x->ref_X()[(i/2) % dim]));
  //     gx[i+1] = to_double(sup(x->ref_X()[(i/2) % dim]));
  //     i += 2;
  //   } while(i % (2*dim));
  // }

  return new GPUFutureImpl<core::arith::ivector>(m_gpu_host->comp_iv(gx, args.size()));
}

template <typename T>
IGPUFuture<core::arith::rvector>* FGPUEval<T>::comp_(const std::vector<core::arith::rvector> &args) const
{
  // unsigned dim = get_f().dim();
  // unsigned size = args.size();
  // double *gx = new double[dim*2*size];

  // for(unsigned i = 0; i < size; i+=m_bsize) {
  //   unsigned rem = std::min(size - i, m_bsize);
  //   unsigned offset = i*dim;
  //   for(unsigned j = 0; j < rem; j++) {
  //     for(unsigned k = 0; k < 2*dim; k++) {
  //       gx[2*j + k*rem+offset] = to_double(args[j+i][k/2]);
  //       //gx[2*j + k*rem+1+offset] = to_double(sup(args[j+i][k/2]));
  //     }
  //   }
  // }

  unsigned dim = get_f().dim();
  double *gx = new double[dim*args.size()];
  unsigned i = 0;
  BOOST_FOREACH(const core::arith::rvector &p, args) {
    assert(size(p)==dim && "Different vector dimensions in input!");
    do {
      gx[i] = to_double(p[i % dim]);
      i++;
    } while(i % (dim));
  }

  unsigned k = 0;
  for(unsigned j = 0; j < args.size()*dim; j+=dim) {
    for(unsigned l =0; l < dim; l++)
      assert(args[k][l] == gx[j+l]);
    k++;
  }


  return new GPUFutureImpl<core::arith::rvector>(m_gpu_host->comp_rv(gx, args.size()));
}

template <typename T>
void FGPUEval<T>::set_gpu_host(IGPUHost *host)
{
  m_gpu_host.reset(host);
}

template <typename T>
void FGPUEval<T>::init()
{
  m_bsize = 1;
}

template <typename T>
void FGPUEval<T>::set_block_size(unsigned bsize)
{
  m_bsize = bsize;
}

template <typename T>
FGPUEval<T>::~FGPUEval()
{}
