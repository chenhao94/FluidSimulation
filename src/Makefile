INCS = -I$(INCS_DIR) \
	   -I/usr/local/include
SRCS = $(wildcard *.cpp)
DEPS = $(patsubst %.cpp,$(DEPS_DIR)/%.d,$(SRCS))
OBJS = $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRCS))
TARGETS = $(TARGETS_DIR)/grid 

.PHONY: all
all: $(TARGETS)

$(TARGETS): $(TARGETS_DIR)/%: $(OBJS)
	@mkdir -p $(TARGETS_DIR)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $@

$(OBJS): $(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

$(DEPS): $(DEPS_DIR)/%.d: %.cpp
	@mkdir -p $(DEPS_DIR)
	$(CXX) $(CXXFLAGS) $(INCS) -MM $< -o $@

ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPS)
endif

.PHONY: clean
clean:
	@$(RM) $(OBJS) $(DEPS) $(TARGETS)
