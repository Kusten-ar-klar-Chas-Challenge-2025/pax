import React from "react";
import { View, Text, TouchableOpacity, StyleSheet } from "react-native";
import { useTheme } from "../theme/ThemeContext";

export default function BookingButton({ onPress }) {
  const { theme } = useTheme();
  const styles = createStyles(theme);
  return (
    <TouchableOpacity
      style={styles.container}
      onPress={onPress}
      activeOpacity={0.8}
    >
      <View style={styles.button}>
        <Text style={styles.text}>PAXA</Text>
      </View>
    </TouchableOpacity>
  );
}

const createStyles = (theme) =>
  StyleSheet.create({
    container: {
      top: -6,
      justifyContent: "center",
      alignItems: "center",
    },
    button: {
      width: 70,
      height: 70,
      borderRadius: 35,
      backgroundColor: "#cef69c",
      justifyContent: "center",
      alignItems: "center",
      shadowColor: "#FFFDCB",
      shadowOffset: { width: 1, height: 1 },
      shadowRadius: 10,
      position: "absolute",
      zIndex: 100,
    },
    text: {
      color: theme.textDark,
      fontFamily: "Nunito",
      fontWeight: "800",
      fontSize: 16,
      lineHeight: 16,
      marginTop: 3,
      letterSpacing: "0.06rem",
      textAlign: "center",
    },
  });
