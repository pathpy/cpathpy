from distutils.core import setup, Extension


def main():
    setup(name="cpathpy",
          version="1.0.0",
          description="Experimental C functions for pathpy",
          author="Jürgen Hackl",
          author_email="hackl@liverpool.ac.uk",
          ext_modules=[Extension("cpathpy", ["tuplehash.c"])])


if __name__ == "__main__":
    main()
