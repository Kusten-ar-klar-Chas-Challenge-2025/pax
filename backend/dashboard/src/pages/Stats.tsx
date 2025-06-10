import { useEffect, useState } from "react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from "recharts";

// Explicitly type usageData
type UsageDatum = {
  date: string;
  [roomName: string]: number | string;
};

const usageData: UsageDatum[] = [
  {
    date: "2025-04-20",
    "Stora Konferensrummet": 5,
    "Lilla Mötesrummet": 2,
    Workshoprummet: 4,
  },
  {
    date: "2025-04-21",
    "Stora Konferensrummet": 7,
    "Lilla Mötesrummet": 3,
    Workshoprummet: 5,
  },
  {
    date: "2025-04-22",
    "Stora Konferensrummet": 4,
    "Lilla Mötesrummet": 4,
    Workshoprummet: 6,
  },
  {
    date: "2025-04-23",
    "Stora Konferensrummet": 6,
    "Lilla Mötesrummet": 1,
    Workshoprummet: 2,
  },
  {
    date: "2025-04-24",
    "Stora Konferensrummet": 3,
    "Lilla Mötesrummet": 5,
    Workshoprummet: 7,
  },
  {
    date: "2025-04-25",
    "Stora Konferensrummet": 8,
    "Lilla Mötesrummet": 2,
    Workshoprummet: 3,
  },
  {
    date: "2025-04-26",
    "Stora Konferensrummet": 6,
    "Lilla Mötesrummet": 3,
    Workshoprummet: 4,
  },
];

const Stats: React.FC = () => {
  const API_BASE_URL = "http://localhost:13000";
  const [stats, setStats] = useState<UsageDatum[]>([]);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const res = await fetch(`${API_BASE_URL}/rooms`);
        const data = await res.json();

        setStats(data);
      } catch (error) {
        console.error("Error fetching data:", error);
      }
    };

    fetchData();
  }, []);
  console.log(stats);

  return (
    <div className="bg-gray-100 min-h-screen p-8">
      <h1 className="text-2xl font-bold mb-8">
        Statistik: Rumsanvändning över tid
      </h1>
      <ResponsiveContainer width="100%" height={400}>
        <LineChart
          data={usageData}
          margin={{ top: 10, right: 30, left: 0, bottom: 0 }}
        >
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="date" />
          <YAxis allowDecimals={false} />
          <Tooltip />
          <Legend />
          <Line
            type="monotone"
            dataKey="Stora Konferensrummet"
            stroke="#8884d8"
            strokeWidth={4}
          />
          <Line
            type="monotone"
            dataKey="Lilla Mötesrummet"
            stroke="#82ca9d"
            strokeWidth={4}
          />
          <Line
            type="monotone"
            dataKey="Workshoprummet"
            stroke="#ffc658"
            strokeWidth={4}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
};

export default Stats;
