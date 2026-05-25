# Makefile con dos binarios: test_tokenizer y test_parser
# Los .o se guardan en obj/ replicando la estructura de src/.

CXX        = c++
CXXFLAGS   = -Wall -Wextra -Werror -std=c++98 -Iinclude

SRC_DIR    = src
OBJ_DIR    = obj

# Archivos comunes a ambos binarios
COMMON_SRCS = config/Token.cpp \
              config/ConfigError.cpp \
              config/ConfigTypes.cpp \
              config/ConfigTokenizer.cpp

# Específicos de cada binario
TOK_SRCS    = test_tokenizer.cpp
PARSER_SRCS = config/ConfigParser.cpp \
			  config/ConfigValidator.cpp \
              test_parser.cpp core/socket.cpp

# Objetos
COMMON_OBJS = $(COMMON_SRCS:%.cpp=$(OBJ_DIR)/%.o)
TOK_OBJS    = $(TOK_SRCS:%.cpp=$(OBJ_DIR)/%.o)
PARSER_OBJS = $(PARSER_SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Binarios
TOK_BIN    = test_tokenizer
PARSER_BIN = test_parser

all: $(TOK_BIN) $(PARSER_BIN)

$(TOK_BIN): $(COMMON_OBJS) $(TOK_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARSER_BIN): $(COMMON_OBJS) $(PARSER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TOK_BIN) $(PARSER_BIN)

re: fclean all

.PHONY: all clean fclean re
