# InterpolationProject
MATH458 - C++ Scientific programming


## Git submodules setup

Make sure to add the required submodules to your git repositery (local):
- googletest
- eigen

The following steps will add the required submodules in the git index:
```bash
mkdir external
cd external

~/external$ git submodule add https://gitlab.com/libeigen/eigen external/eigen
~/external$ git submodule add https://github.com/google/googletest.git external/googletest
```

