Import("env")

# Apply C++-only flags
env.Append(
    CXXFLAGS=[
        "-fno-exceptions",
        "-fno-rtti"
    ]
)