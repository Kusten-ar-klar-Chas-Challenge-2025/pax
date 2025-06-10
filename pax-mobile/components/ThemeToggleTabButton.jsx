import React from "react";
import { Pressable, Text, View, StatusBar, StyleSheet } from "react-native";
import { MaterialCommunityIcons } from "@expo/vector-icons";
import { useTheme } from "../theme/ThemeContext";

const ThemeToggleTabButton = ({ onPress, accessibilityState }) => {
  const { isDark, toggleTheme, theme } = useTheme();
  const focused = accessibilityState?.selected;

  return (
    <>
      <StatusBar
        barStyle={isDark ? "light-content" : "dark-content"}
        backgroundColor={theme.headerBackground}
      />
      <View style={styles.container}>
        <Pressable
          onPress={toggleTheme}
          accessible={true}
          accessibilityRole="button"
          accessibilityLabel={
            isDark ? "Byt till ljust tema" : "Byt till mörkt tema"
          }
          style={styles.toggleButton}
        >
          <MaterialCommunityIcons
            name={isDark ? "white-balance-sunny" : "weather-night"}
            size={30}
            color={theme.iconSwitchmode}
          />
        </Pressable>
      </View>
    </>
  );
};

const styles = StyleSheet.create({
  container: {
    alignItems: "center",
    justifyContent: "center",
    padding: 10,
  },
  toggleButton: {
    position: "absolute",
    top: 10,
    left: 10,
    zIndex: 1,
    backgroundColor: "#E3FAC825",
    borderRadius: 25,
    padding: 5,
  },
});

export default ThemeToggleTabButton;
