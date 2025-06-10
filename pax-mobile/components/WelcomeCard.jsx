import React from "react";
import { View, Text, StyleSheet } from "react-native";
import { useTheme } from "../theme/ThemeContext";

const WelcomeCard = () => {
  const { theme } = useTheme();
  const styles = getStyles(theme);

  const days = [
    "Söndag",
    "Måndag",
    "Tisdag",
    "Onsdag",
    "Torsdag",
    "Fredag",
    "Lördag",
  ];
  const today = new Date();
  const currentDay = days[today.getDay()];

  return (
    <View style={styles.card}>
      <Text style={styles.title}>Hej!</Text>
      <Text style={styles.subtitle}>
        {`Idag är det ${currentDay}.\n\nHoppas du får en fin dag!`}
      </Text>
    </View>
  );
};

export default WelcomeCard;

const getStyles = (theme) =>
  StyleSheet.create({
    card: {
      padding: 20,
      margin: 20,
      borderRadius: 12,
      backgroundColor: theme.card,
      elevation: 4,
      alignItems: "center",
      marginTop: 60,
    },
    title: {
      fontSize: 20,
      fontWeight: "800",
      marginBottom: 6,
      textAlign: "center",
      color: theme.textPrimary,
      fontFamily: "NunitoSans",
    },
    subtitle: {
      fontSize: 15,
      fontWeight: "400",
      color: theme.text,
      fontFamily: "NunitoSans",
      color: theme.textPrimary,
      textAlign: "center",
    },
  });
