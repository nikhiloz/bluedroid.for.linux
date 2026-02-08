## Description
**What does this PR do?**
<!-- Clear, concise description of changes -->

## Related Issues
Fixes #123
Related to #456

## Type of Change
- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to change)
- [ ] Documentation update
- [ ] Performance improvement
- [ ] Refactoring

## Changes Made
**Detailed list of modifications:**
- Changed X in `file.c` to improve Y
- Added new function `example()` in `module/`
- Updated documentation in README.md

## Testing
**How was this tested?**
```bash
# Build command
make clean && make -j$(nproc)

# Tests run
make test

# Manual testing steps
1. ...
2. ...
```

- [ ] Unit tests added/updated
- [ ] Manual testing completed
- [ ] No regressions observed
- [ ] Valgrind check passed (if applicable)

## Checklist
- [ ] Code follows project style guidelines (K&R, 4-space indent)
- [ ] Comments added for complex logic
- [ ] Documentation updated (README, docs/, etc.)
- [ ] No new compiler warnings (`-Wall -Werror`)
- [ ] No breaking changes to public APIs
- [ ] License header added to new files
- [ ] Commit messages are clear and descriptive

## Performance Impact
- **Memory:** Negligible / +X bytes / -X bytes
- **Speed:** No change / +X% / -X%
- **Startup time:** Not affected / +X ms / -X ms

## Breaking Changes
- [ ] No breaking changes
- [ ] Breaking changes (describe migration path):

## Screenshots/Logs
<!-- If applicable, add evidence of changes -->
```
Relevant logs or output here
```

## Additional Notes
**Any other context?**
- Affects multiple platforms (list):
- Requires config changes:
- Known limitations:

## Reviewer Notes
@maintainers - Please pay attention to:
- Critical section: X
- Potential issue: Y
- Need approval from: [specific expert]

---

**Closes:** Fixes #[issue_number]  
**CC:** @[related_person]
