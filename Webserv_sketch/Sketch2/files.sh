for dir in */; do
    # Check if it's a directory
    if [ -d "$dir" ]; then
        # Create .cpp and .hpp files in each subfolder
        touch "${dir}${dir%/}.cpp"
        touch "${dir}${dir%/}.hpp"
    fi
done