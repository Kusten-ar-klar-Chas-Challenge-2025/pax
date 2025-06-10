import { StyleSheet, Text, View } from 'react-native'
import React from 'react'
import { useTheme } from '../theme/ThemeContext'

const CardComponent = ({ title, description }) => {
    const { theme } = useTheme();
    const styles = createStyles(theme);


    return (
            <View style={styles.card}>
                <Text style={styles.title}>Hej Användaren</Text>
                <Text style={styles.description}>
                    Ditt bokade rum: <b>"Alfa"</b><br/>På våning:<b> 4</b>
                    <br/>Är ledigt om:<b> 3:26 min</b>
                </Text>
                <Text style={styles.greeting}>Ha en bra dag!</Text>
            </View>
    )
};

export default CardComponent

const createStyles = (theme) =>
    StyleSheet.create({
        card: {
            backgroundColor: theme.card,
            height: 240,
            width: 320,
            padding: 18,
            borderBottomRightRadius: 30,
            borderTopLeftRadius: 30,
            elevation: 3,
            bottom: 20,
        },
        title: {
            color: "#B5DA87",
            fontFamily: "BadScript",
            fontSize: 28,
            fontWeight: 500,
            marginBottom: 8,
           letterSpacing: -0.45,
        },
        description: {
            color: theme.textSecondary,
            fontFamily: "NunitoSans",
            fontSize: 15,
            lineHeight: 26,
            fontWeight: 500,
          },
          greeting: {
            color: theme.textSecondary,
            fontFamily: "BadScript",
            textAlign: "right",
            fontSize: 20,
            fontWeight: 800,
            marginBottom: 1,
            marginTop: 28,
            letterSpacing: -0.35,
          },
    })

// const styles = StyleSheet.create({
//     card: {
//         borderRadius: 12,
//         padding: 20, 
//         width: 260,
//         height: 160, 
//         marginBottom: 14,
//         marginHorizontal: 14

//     }
// })