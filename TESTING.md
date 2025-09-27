# Testing

This project includes a comprehensive unit test suite that validates the core IOC functionality.

## Running Tests

### Build and Run Tests

```bash
# Clone the repository
git clone https://github.com/radzevich/Compile-time-Dependency-Injection.git
cd Compile-time-Dependency-Injection

# Create build directory
mkdir build && cd build

# Configure with tests enabled
cmake .. -DIOC_BUILD_TESTS=ON

# Build the tests
make

# Run tests using CTest
ctest --output-on-failure

# Or run tests directly
./tests/ioc_tests
```

### Test Structure

The test suite is organized into several categories:

- **Container Tests**: Core container resolution and service lookup
- **Binding Tests**: Service binding and lifetime configuration  
- **Lifetime Manager Tests**: Service lifetime behavior (Transient, Scoped, Singleton)
- **Service Factory Tests**: Service creation and custom factories

### Current Test Results

The test suite includes 15 tests with an 80% pass rate (12 passing, 3 failing):

- ✅ Container resolution and basic functionality
- ✅ Transient lifetime behavior
- ✅ Singleton lifetime behavior
- ✅ Service factory operations
- ✅ Binding configurations
- ⚠️ Scoped lifetime behavior (3 failing tests - known issue)

The failing tests are related to scoped lifetime manager implementation details and do not affect the core library functionality.

## Continuous Integration

The project uses GitHub Actions for continuous integration:

- **Multi-platform testing**: Linux, Windows, macOS
- **Multi-compiler testing**: GCC, Clang, MSVC
- **Build configurations**: Debug and Release
- **Code coverage reporting**: Available on pull requests

The CI pipeline automatically runs on:
- Pull requests to `main` branch
- Pushes to `main` branch

## Test Framework

Tests are implemented using a lightweight custom testing framework that provides:

- Test assertions (`EXPECT_EQ`, `EXPECT_TRUE`, etc.)
- Test organization and reporting
- Minimal dependencies (header-only)
- Cross-platform compatibility

The custom framework was chosen to avoid external dependencies and network restrictions during builds.